#include "metabc.h"
#include <cmath>

void MetabForwardEulerDo::run()
{
   // Set the initial oxygen concentration
   outputDo.dox[0] = initialDO;

   // Calculate effects on effective organic carbon
   parDist[0] = parDistCalculator.calc(
      dt[0],
      par[0]
   );
   output.cFixation[0] = dailyGPP * parDist[0];
   output.cRespiration[0] = dailyER * dt[0];

   outputDo.doProduction[0] =
      output.cFixation[0] * ratioDoCFix;
   outputDo.doConsumption[0] =
      output.cRespiration[0] * ratioDoCResp;

   kDo[0] = kSchmidtDoCalculator(temp[0], k600);
   outputDo.doEquilibration[0] =
      dt[0] * kDo[0] * (satDo[0] - outputDo.dox[0]);

   // Loop through time steps
   int lastIndex = length - 1;
   for (int i = 1; i < lastIndex; i++) {
      outputDo.dox[i] =
         outputDo.dox[i - 1] +
         outputDo.doProduction[i - 1] +
         outputDo.doConsumption[i - 1] +
         outputDo.doEquilibration[i - 1];

      parDist[i] = parDistCalculator.calc(
         dt[i],
         par[i]
      );
      output.cFixation[i] = dailyGPP * parDist[i];
      output.cRespiration[i] = dailyER * dt[i];

      outputDo.doProduction[i] =
         output.cFixation[i] * ratioDoCFix;
      outputDo.doConsumption[i] =
         output.cRespiration[i] * ratioDoCResp;

      kDo[i] = kSchmidtDoCalculator(temp[i], k600);
      outputDo.doEquilibration[i] =
         dt[i] * kDo[i] * (satDo[i] - outputDo.dox[i]);
   }

   outputDo.dox[lastIndex] =
      outputDo.dox[lastIndex - 1] +
      outputDo.doProduction[lastIndex - 1] +
      outputDo.doConsumption[lastIndex - 1] +
      outputDo.doEquilibration[lastIndex - 1];

   parDist[lastIndex] = 0;
   output.cFixation[lastIndex] = 0;
   output.cRespiration[lastIndex] = 0;

   outputDo.doConsumption[lastIndex] = 0;
   outputDo.doProduction[lastIndex] = 0;

   kDo[lastIndex] = kSchmidtDoCalculator(temp[lastIndex], k600);
   outputDo.doEquilibration[lastIndex] = 0;

}
