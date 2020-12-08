#include "metabc.h"
#include <cmath>

void MetabCrankNicolsonDo::run()
{
   outputDo.dox[0] = initialDO;

   kDo[0] = 0.5 * (
      kSchmidtDoCalculator(temp[0], k600) +
      kSchmidtDoCalculator(temp[1], k600)
   );

   double parAvg = 0.5 * (par[0] + par[1]);

   output.cFixation[0] = dailyGPP *
      ((parAvg * dt[0]) / parTotal);
   output.cRespiration[0] = dailyER * dt[0];

   outputDo.doProduction[0] =
      output.cFixation[0] * ratioDoCFix;
   outputDo.doConsumption[0] =
      output.cRespiration[0] * ratioDoCResp;
   outputDo.doEquilibration[0] =
      dt[0] * kDo[0] *
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
            1 + (0.5 * (dt[i - 1] * kDo[i - 1]))
         );

      kDo[i] = 0.5 * (
         kSchmidtDoCalculator(temp[i], k600) +
         kSchmidtDoCalculator(temp[i + 1], k600)
      );

      parAvg = 0.5 * (par[i] + par[i + 1]);

      output.cFixation[i] = dailyGPP *
         ((parAvg * dt[i]) / parTotal);
      output.cRespiration[i] = dailyER * dt[i];

      outputDo.doProduction[i] =
         output.cFixation[i] * ratioDoCFix;
      outputDo.doConsumption[i] =
         output.cRespiration[i] * ratioDoCResp;
      outputDo.doEquilibration[i] =
         dt[i] * kDo[i] *
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
         1 + (0.5 * (dt[lastIndex - 1] * kDo[lastIndex - 1]))
      );

   kDo[lastIndex] = kSchmidtDoCalculator(temp[lastIndex], k600);

   output.cFixation[lastIndex] = dailyGPP *
      ((par[lastIndex] * dt[lastIndex]) / parTotal);
   output.cRespiration[lastIndex] = dailyER * dt[lastIndex];

   outputDo.doProduction[lastIndex] =
      output.cFixation[lastIndex] * ratioDoCFix;
   outputDo.doConsumption[lastIndex] =
      output.cRespiration[lastIndex] * ratioDoCResp;
   outputDo.doEquilibration[lastIndex] =
      dt[lastIndex] * kDo[lastIndex] *
      (satDo[lastIndex] - outputDo.dox[lastIndex]);
}
