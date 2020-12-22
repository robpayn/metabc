#include "metabc.h"
#include <cmath>

void MetabLagrangeCNOneStepDoDic::run()
{
   MetabLagrangeCNOneStepDo::run();

   for(int i = 0; i < numParcels; i++) {
      double upstreamDeficit =
         upstreamSatCO2[i] - (upstreamkH[i] * upstreampCO2[i]);

      outputDic.dicProduction[i] =
         output.cRespiration[i] * ratioDicCResp;
      outputDic.dicConsumption[i] =
         output.cFixation[i] * ratioDicCFix;

      upstreamkCO2[i] = kSchmidtCO2Calculator(upstreamTemp[i], k600);
      downstreamkCO2[i] = kSchmidtCO2Calculator(downstreamTemp[i], k600);
      double avgkCO2 = 0.5 * (upstreamkCO2[i] + downstreamkCO2[i]);
      outputDic.co2Equilibration[i] =
         travelTimes[i] *
         avgkCO2 *
         0.5 * (upstreamDeficit + downstreamSatCO2[i]);

      carbonateEq.reset(downstreamTemp[i], 0);

      proposeDic_info info;
      info.carbonateEq = &carbonateEq;
      info.alkalinity = downstreamAlkalinity[i];
      info.kCO2 = avgkCO2;
      info.dt = travelTimes[i];
      info.target =
         upstreamDIC[i] +
         outputDic.dicProduction[i] +
         outputDic.dicConsumption[i] +
         outputDic.co2Equilibration[i];

      outputDic.dic[i] = Brent_fmin(
         minDIC,
         maxDIC,
         proposeDic,
         &info,
         tolerance
      );

      double dicOptim[2];
      carbonateEq.optfCO2FromDICTotalAlk(
         outputDic.dic[i] * 1e-6,
         downstreamAlkalinity[i] * 1e-6,
         1e-5,
         2,
         12,
         dicOptim
      );
      outputDic.pCO2[i] = dicOptim[1];
      outputDic.pH[i] = dicOptim[0];
   }
}
