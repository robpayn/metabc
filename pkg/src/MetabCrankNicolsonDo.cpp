#include "metabc.h"
#include <cmath>

void MetabCrankNicolsonDo::run()
{
   // Set the initial oxygen concentration
   outputDo.dox[0] = initialDO;

   // Calculate effects on effective organic carbon
   parDist[0] = parDistCalculator.calc(
      dt[0],
      parAvg[0]
   );
   output.cFixation[0] = dailyGPP * parDist[0];
   output.cRespiration[0] = dailyER * dt[0];

   double avgkDO = 0.5 * (kDo[0] + kDo[1]);
   outputDo.doProduction[0] =
      output.cFixation[0] * ratioDoCFix;
   outputDo.doConsumption[0] =
      output.cRespiration[0] * ratioDoCResp;
   outputDo.doEquilibration[0] =
      dt[0] *
      avgkDO *
      0.5 * (satDo[0] - outputDo.dox[0] + satDo[1]);

   // Loop through time steps
   for (int i = 1; i < length - 1; i++) {
      outputDo.dox[i] =
         (
            outputDo.dox[i - 1] +
            outputDo.doProduction[i - 1] +
            outputDo.doConsumption[i - 1] +
            outputDo.doEquilibration[i - 1]
         ) /
         (
            1 + (0.5 * (dt[i - 1] * avgkDO))
         );

      parDist[i] = parDistCalculator.calc(
         dt[i],
         parAvg[i]
      );
      output.cFixation[i] = dailyGPP * parDist[i];
      output.cRespiration[i] = dailyER * dt[i];

      avgkDO = 0.5 * (kDo[i] + kDo[i + 1]);
      outputDo.doProduction[i] =
         output.cFixation[i] * ratioDoCFix;
      outputDo.doConsumption[i] =
         output.cRespiration[i] * ratioDoCResp;
      outputDo.doEquilibration[i] =
         dt[i] *
         avgkDO *
         0.5 * (satDo[i] - outputDo.dox[i] + satDo[i + 1]);
   }

   int lastIndex = length - 1;
   outputDo.dox[lastIndex] =
      (
         outputDo.dox[lastIndex - 1] +
         outputDo.doProduction[lastIndex - 1] +
         outputDo.doConsumption[lastIndex - 1] +
         outputDo.doEquilibration[lastIndex - 1]
      ) /
      (
         1 + (0.5 * (dt[lastIndex - 1] * avgkDO))
      );

   parDist[lastIndex] = 0;
   output.cFixation[lastIndex] = 0;
   output.cRespiration[lastIndex] = 0;

   outputDo.doProduction[lastIndex] = 0;
   outputDo.doConsumption[lastIndex] = 0;
   outputDo.doEquilibration[lastIndex] = 0;
}
