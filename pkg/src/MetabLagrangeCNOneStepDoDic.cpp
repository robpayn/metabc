#include "metabc.h"
#include <cmath>

void MetabLagrangeCNOneStepDoDic::run()
{
   MetabLagrangeCNOneStepDo::run();

   for(int i = 0; i < numParcels_; i++) {
      double upstreamDeficit =
         upstreamSatCO2_[i] - (upstreamkH_[i] * upstreampCO2_[i]);

      outputDic_.dicProduction[i] =
         output_.cRespiration[i] * ratioDicCResp_;
      outputDic_.dicConsumption[i] =
         output_.cFixation[i] * ratioDicCFix_;

      upstreamkCO2_[i] = kSchmidtCO2Calculator_(upstreamTemp_[i], k600_);
      downstreamkCO2_[i] = kSchmidtCO2Calculator_(downstreamTemp_[i], k600_);
      double avgkCO2 = 0.5 * (upstreamkCO2_[i] + downstreamkCO2_[i]);
      outputDic_.co2Equilibration[i] =
         travelTimes_[i] *
         avgkCO2 *
         0.5 * (upstreamDeficit + downstreamSatCO2_[i]);

      carbonateEq_.reset(downstreamTemp_[i], 0);

      proposeDic_info info;
      info.carbonateEq = &carbonateEq_;
      info.alkalinity = downstreamAlkalinity_[i];
      info.kCO2 = avgkCO2;
      info.dt = travelTimes_[i];
      info.target =
         upstreamDIC_[i] +
         outputDic_.dicProduction[i] +
         outputDic_.dicConsumption[i] +
         outputDic_.co2Equilibration[i];

      outputDic_.dic[i] = Brent_fmin(
         minDIC,
         maxDIC,
         proposeDic,
         &info,
         tolerance
      );

      double dicOptim[2];
      carbonateEq_.optfCO2FromDICTotalAlk(
         outputDic_.dic[i] * 1e-6,
         downstreamAlkalinity_[i] * 1e-6,
         1e-5,
         2,
         12,
         dicOptim
      );
      outputDic_.pCO2[i] = dicOptim[1];
      outputDic_.pH[i] = dicOptim[0];
   }
}
