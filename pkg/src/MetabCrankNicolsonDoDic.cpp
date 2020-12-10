#include "metabc.h"
#include <cmath>
#include "utilities.h"

struct proposeDic_info
{
   CarbonateEq* carbonateEq;
   double alkalinity;
   double kCO2;
   double dt;
   double target;
};

double proposeDic(double dic, void* info)
{
   proposeDic_info* p = (proposeDic_info*)info;
   double dicOptim[2];
   p->carbonateEq->optfCO2FromDICTotalAlk(
      dic * 1e-6,
      p->alkalinity * 1e-6,
      1e-5,
      2,
      12,
      dicOptim
   );
   double CO2 = dicOptim[1] * p->carbonateEq->kHenryCO2;
   double guess = dic + p->kCO2 * p->dt * CO2 * 0.5;
   return fabs(p->target - guess);
}

void MetabCrankNicolsonDoDic::run()
{
   // Run the base class DO model
   MetabCrankNicolsonDo::run();

   // Set the first elements for henry's coefficient and DIC
   carbonateEq.reset(temp[0], 0);
   kH[0] = carbonateEq.kHenryCO2;

   outputDic.dic[0] = initialDIC;

   // Run the carbonate equilibrium for initial pH and pCO2
   double dicOptim[2];
   carbonateEq.optfCO2FromDICTotalAlk(
      outputDic.dic[0] * 1e-6,
      alkalinity[0] * 1e-6,
      1e-5,
      2,
      12,
      dicOptim
   );
   outputDic.pCO2[0] = dicOptim[1];
   outputDic.pH[0] = dicOptim[0];

   double lastCO2Deficit =
      kH[0] * (pCO2air[0] - outputDic.pCO2[0]);
   carbonateEq.reset(temp[1], 0);
   double nextCO2Sat = carbonateEq.kHenryCO2 * pCO2air[1];

   // Calculate initial dic inputs and outputs
   outputDic.dicProduction[0] =
      output.cRespiration[0] * ratioDicCresp;
   outputDic.dicConsumption[0] =
      output.cFixation[0] * ratioDicCfix;
   outputDic.co2Equilibration[0] =
      dt[0] *
      0.5 * (kCO2[0] + kCO2[1]) *
      0.5 * (lastCO2Deficit + nextCO2Sat);

   for(int i = 1; i < length - 1; i++) {
      kH[i] = carbonateEq.kHenryCO2;

      proposeDic_info info;
      info.carbonateEq = &carbonateEq;
      info.alkalinity = alkalinity[i];
      info.kCO2 = kCO2[i];
      info.dt = dt[i];
      info.target =
         outputDic.dic[i - 1] +
         outputDic.dicProduction[i - 1] +
         outputDic.dicConsumption[i - 1] +
         outputDic.co2Equilibration[i - 1];

      outputDic.dic[i] = Brent_fmin(
         minDIC,
         maxDIC,
         proposeDic,
         &info,
         tolerance
      );

      carbonateEq.optfCO2FromDICTotalAlk(
         outputDic.dic[i] * 1e-6,
         alkalinity[i] * 1e-6,
         1e-5,
         2,
         12,
         dicOptim
      );
      outputDic.pCO2[i] = dicOptim[1];
      outputDic.pH[i] = dicOptim[0];

      lastCO2Deficit =
         kH[i] * (pCO2air[i] - outputDic.pCO2[i]);
      carbonateEq.reset(temp[i + 1], 0);
      nextCO2Sat = carbonateEq.kHenryCO2 * pCO2air[i + 1];

      outputDic.dicProduction[i] =
         output.cRespiration[i] * ratioDicCresp;
      outputDic.dicConsumption[i] =
         output.cFixation[i] * ratioDicCfix;
      outputDic.co2Equilibration[i] =
         dt[i] *
         0.5 * (kCO2[i] + kCO2[i + 1]) *
         0.5 * (lastCO2Deficit + nextCO2Sat);
   }

   int lastIndex = length - 1;

   kH[lastIndex] = carbonateEq.kHenryCO2;

   proposeDic_info info;
   info.carbonateEq = &carbonateEq;
   info.alkalinity = alkalinity[lastIndex];
   info.kCO2 = kCO2[lastIndex];
   info.dt = dt[lastIndex];
   info.target =
      outputDic.dic[lastIndex - 1] +
      outputDic.dicProduction[lastIndex - 1] +
      outputDic.dicConsumption[lastIndex - 1] +
      outputDic.co2Equilibration[lastIndex - 1];

   outputDic.dic[lastIndex] = Brent_fmin(
      minDIC,
      maxDIC,
      proposeDic,
      &info,
      tolerance
   );

   carbonateEq.optfCO2FromDICTotalAlk(
      outputDic.dic[lastIndex] * 1e-6,
      alkalinity[lastIndex] * 1e-6,
      1e-5,
      2,
      12,
      dicOptim
   );
   outputDic.pCO2[lastIndex] = dicOptim[1];
   outputDic.pH[lastIndex] = dicOptim[0];

   outputDic.dicProduction[lastIndex] = 0;
   outputDic.dicConsumption[lastIndex] = 0;
   outputDic.co2Equilibration[lastIndex] = 0;
}
