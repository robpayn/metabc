#include "metabc.h"
#include <cmath>

void MetabLagrangeCNOneStepDo::run()
{
   // Loop through time steps
   for (int i = 0; i < numParcels; i++) {
      parDist[i] = parDistCalculator.calc(
         travelTimes[i],
         parAvg[i]
      );
      output.cFixation[i] = dailyGPP * parDist[i];
      output.cRespiration[i] = dailyER * travelTimes[i];

      double avgkDO = 0.5 * (upstreamkDo[i] + downstreamkDo[i]);
      outputDo.doProduction[i] =
         output.cFixation[i] * ratioDoCFix;
      outputDo.doConsumption[i] =
         output.cRespiration[i] * ratioDoCResp;
      outputDo.doEquilibration[i] =
         travelTimes[i] *
         avgkDO *
         0.5 * (upstreamSatDo[i] - upstreamDO[i] + downstreamSatDo[i]);

      outputDo.dox[i] =
         (
            upstreamDO[i] +
            outputDo.doProduction[i] +
            outputDo.doConsumption[i] +
            outputDo.doEquilibration[i]
         ) /
         (
            1 + (0.5 * (travelTimes[i] * avgkDO))
         );
   }

}
