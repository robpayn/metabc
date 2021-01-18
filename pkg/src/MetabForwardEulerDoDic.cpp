#include "metabc.h"
#include <cmath>

void MetabForwardEulerDoDic::run()
{
   // Run the base class DO model
   MetabForwardEulerDo::run();

   // Set the first elements for henry's coefficient and DIC
   carbonateEq_.reset(temp_[0], 0);
   kH_[0] = carbonateEq_.kHenryCO2;
   outputDic_.dic[0] = initialDIC_;

   // Run the carbonate equilibrium for initial pH and pCO2
   double dicOptim[2];
   carbonateEq_.optfCO2FromDICTotalAlk(
      outputDic_.dic[0] * 1e-6,
      alkalinity_[0] * 1e-6,
      1e-5,
      2,
      12,
      dicOptim
   );
   outputDic_.pCO2[0] = dicOptim[1];
   outputDic_.pH[0] = dicOptim[0];

   // Calculate initial dic inputs and outputs
   outputDic_.dicProduction[0] =
      output_.cRespiration[0] * ratioDicCResp_;
   outputDic_.dicConsumption[0] =
      output_.cFixation[0] * ratioDicCFix_;

   kCO2_[0] = kSchmidtCO2Calculator_(temp_[0], k600_);
   outputDic_.co2Equilibration[0] =
      dt_[0] * kCO2_[0] *
      kH_[0] * (pCO2air_[0] - outputDic_.pCO2[0]);

   int lastIndex = length_ - 1;
   for(int i = 1; i < lastIndex; i++) {
      carbonateEq_.reset(temp_[i], 0);
      kH_[i] = carbonateEq_.kHenryCO2;

      long prevIndex = i - 1;
      outputDic_.dic[i] =
         outputDic_.dic[prevIndex] +
         outputDic_.dicProduction[prevIndex] +
         outputDic_.dicConsumption[prevIndex] +
         outputDic_.co2Equilibration[prevIndex];

      carbonateEq_.optfCO2FromDICTotalAlk(
         outputDic_.dic[i] * 1e-6,
         alkalinity_[i] * 1e-6,
         1e-5,
         2,
         12,
         dicOptim
      );
      outputDic_.pCO2[i] = dicOptim[1];
      outputDic_.pH[i] = dicOptim[0];

      outputDic_.dicProduction[i] =
         output_.cRespiration[i] * ratioDicCResp_;
      outputDic_.dicConsumption[i] =
         output_.cFixation[i] * ratioDicCFix_;

      kCO2_[i] = kSchmidtCO2Calculator_(temp_[i], k600_);
      outputDic_.co2Equilibration[i] =
         dt_[i] * kCO2_[i] *
         kH_[i] * (pCO2air_[i] - outputDic_.pCO2[i]);
   }

   carbonateEq_.reset(temp_[lastIndex], 0);
   kH_[lastIndex] = carbonateEq_.kHenryCO2;

   long prevLastIndex = lastIndex - 1;
   outputDic_.dic[lastIndex] =
      outputDic_.dic[prevLastIndex] +
      outputDic_.dicProduction[prevLastIndex] +
      outputDic_.dicConsumption[prevLastIndex] +
      outputDic_.co2Equilibration[prevLastIndex];

   carbonateEq_.optfCO2FromDICTotalAlk(
      outputDic_.dic[lastIndex] * 1e-6,
      alkalinity_[lastIndex] * 1e-6,
      1e-5,
      2,
      12,
      dicOptim
   );
   outputDic_.pCO2[lastIndex] = dicOptim[1];
   outputDic_.pH[lastIndex] = dicOptim[0];

   outputDic_.dicProduction[lastIndex] = 0;
   outputDic_.dicConsumption[lastIndex] = 0;

   kCO2_[lastIndex] = kSchmidtCO2Calculator_(temp_[lastIndex], k600_);
   outputDic_.co2Equilibration[lastIndex] = 0;
}
