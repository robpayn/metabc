#include "metabc.h"
#include <cmath>

void MetabForwardEulerDoDic::run()
{
   // Run the base class DO model
   MetabForwardEulerDo::run();

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

   // Calculate initial dic inputs and outputs
   outputDic.dicProduction[0] =
      output.cRespiration[0] * ratioDicCResp;
   outputDic.dicConsumption[0] =
      output.cFixation[0] * ratioDicCFix;

   kCO2[0] = kSchmidtCO2Calculator(temp[0], k600);
   outputDic.co2Equilibration[0] =
      dt[0] * kCO2[0] *
      kH[0] * (pCO2air[0] - outputDic.pCO2[0]);

   int lastIndex = length - 1;
   for(int i = 1; i < lastIndex; i++) {
      carbonateEq.reset(temp[i], 0);
      kH[i] = carbonateEq.kHenryCO2;

      outputDic.dic[i] =
         outputDic.dic[i - 1] +
         outputDic.dicProduction[i - 1] +
         outputDic.dicConsumption[i - 1] +
         outputDic.co2Equilibration[i - 1];

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

      outputDic.dicProduction[i] =
         output.cRespiration[i] * ratioDicCResp;
      outputDic.dicConsumption[i] =
         output.cFixation[i] * ratioDicCFix;

      kCO2[i] = kSchmidtCO2Calculator(temp[i], k600);
      outputDic.co2Equilibration[i] =
         dt[i] * kCO2[i] *
         kH[i] * (pCO2air[i] - outputDic.pCO2[i]);
   }

   carbonateEq.reset(temp[lastIndex], 0);
   kH[lastIndex] = carbonateEq.kHenryCO2;

   outputDic.dic[lastIndex] =
      outputDic.dic[lastIndex - 1] +
      outputDic.dicProduction[lastIndex - 1] +
      outputDic.dicConsumption[lastIndex - 1] +
      outputDic.co2Equilibration[lastIndex - 1];

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

   kCO2[lastIndex] = kSchmidtCO2Calculator(temp[lastIndex], k600);
   outputDic.co2Equilibration[lastIndex] = 0;
}
