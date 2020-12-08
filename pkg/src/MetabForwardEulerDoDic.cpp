#include "metabc.h"
#include <cmath>

void MetabForwardEulerDoDic::run()
{
   // Run the base class DO model
   MetabForwardEulerDo::run();

   // Set the first elements for henry's coefficient and DIC
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
      output.cRespiration[0] * ratioDicCresp;
   outputDic.dicConsumption[0] =
      output.cFixation[0] * ratioDicCfix;
   outputDic.co2Equilibration[0] =
      dt[0] * kCO2[0] *
      kH[0] * (pCO2air[0] - outputDic.pCO2[0]);

   for(int i = 1; i < length; i++) {
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
         output.cRespiration[i] * ratioDicCresp;
      outputDic.dicConsumption[i] =
         output.cFixation[i] * ratioDicCfix;
      outputDic.co2Equilibration[i] =
         dt[i] * kCO2[i] *
         kH[i] * (pCO2air[i] - outputDic.pCO2[i]);
   }
}
