#include "metabc.h"
#include <cmath>

void MetabCrankNicolsonDoDic::run()
{
   // Run the base class DO model
   MetabCrankNicolsonDo::run();

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

   double lastCO2Deficit =
      kH_[0] * (pCO2air_[0] - outputDic_.pCO2[0]);
   carbonateEq_.reset(temp_[1], 0);
   double nextCO2Sat = carbonateEq_.kHenryCO2 * pCO2air_[1];

   // Calculate initial dic inputs and outputs
   outputDic_.dicProduction[0] =
      output_.cRespiration[0] * ratioDicCResp_;
   outputDic_.dicConsumption[0] =
      output_.cFixation[0] * ratioDicCFix_;

   kCO2_[0] = kSchmidtCO2Calculator_(temp_[0], k600_);
   kCO2_[1] = kSchmidtCO2Calculator_(temp_[1], k600_);
   double avgkCO2 = 0.5 * (kCO2_[0] + kCO2_[1]);
   outputDic_.co2Equilibration[0] =
      dt_[0] *
      avgkCO2 *
      0.5 * (lastCO2Deficit + nextCO2Sat);

   int lastIndex = length_ - 1;
   for(int i = 1; i < lastIndex; i++) {
      kH_[i] = carbonateEq_.kHenryCO2;

      long prevIndex = i - 1;
      proposeDic_info info;
      info.carbonateEq = &carbonateEq_;
      info.alkalinity = alkalinity_[i];
      info.kCO2 = avgkCO2;
      info.dt = dt_[prevIndex];
      info.target =
         outputDic_.dic[prevIndex] +
         outputDic_.dicProduction[prevIndex] +
         outputDic_.dicConsumption[prevIndex] +
         outputDic_.co2Equilibration[prevIndex];

      outputDic_.dic[i] = Brent_fmin(
         minDIC,
         maxDIC,
         proposeDic,
         &info,
         tolerance
      );

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

      lastCO2Deficit =
         kH_[i] * (pCO2air_[i] - outputDic_.pCO2[i]);
      carbonateEq_.reset(temp_[i + 1], 0);
      nextCO2Sat = carbonateEq_.kHenryCO2 * pCO2air_[i + 1];

      outputDic_.dicProduction[i] =
         output_.cRespiration[i] * ratioDicCResp_;
      outputDic_.dicConsumption[i] =
         output_.cFixation[i] * ratioDicCFix_;

      kCO2_[i + 1] = kSchmidtCO2Calculator_(temp_[i + 1], k600_);
      avgkCO2 = 0.5 * (kCO2_[i] + kCO2_[i + 1]);
      outputDic_.co2Equilibration[i] =
         dt_[i] *
         avgkCO2 *
         0.5 * (lastCO2Deficit + nextCO2Sat);
   }

   kH_[lastIndex] = carbonateEq_.kHenryCO2;

   long prevLastIndex = lastIndex - 1;
   proposeDic_info info;
   info.carbonateEq = &carbonateEq_;
   info.alkalinity = alkalinity_[lastIndex];
   info.kCO2 = avgkCO2;
   info.dt = dt_[prevLastIndex];
   info.target =
      outputDic_.dic[prevLastIndex] +
      outputDic_.dicProduction[prevLastIndex] +
      outputDic_.dicConsumption[prevLastIndex] +
      outputDic_.co2Equilibration[prevLastIndex];

   outputDic_.dic[lastIndex] = Brent_fmin(
      minDIC,
      maxDIC,
      proposeDic,
      &info,
      tolerance
   );

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
   outputDic_.co2Equilibration[lastIndex] = 0;
}
