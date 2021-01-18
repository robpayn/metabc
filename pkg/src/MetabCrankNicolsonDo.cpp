#include "metabc.h"
#include <cmath>

void MetabCrankNicolsonDo::run()
{
   // Set the initial oxygen concentration
   outputDo_.dox[0] = initialDO_;

   // Calculate effects on effective organic carbon
   parDist_[0] = parDistCalculator_.calc(
      dt_[0],
      parAvg_[0]
   );
   output_.cFixation[0] = dailyGPP_ * parDist_[0];
   output_.cRespiration[0] = dailyER_ * dt_[0];

   outputDo_.doProduction[0] =
      output_.cFixation[0] * ratioDoCFix_;
   outputDo_.doConsumption[0] =
      output_.cRespiration[0] * ratioDoCResp_;

   kDo_[0] = kSchmidtDoCalculator_(temp_[0], k600_);
   kDo_[1] = kSchmidtDoCalculator_(temp_[1], k600_);
   double avgkDO = 0.5 * (kDo_[0] + kDo_[1]);
   outputDo_.doEquilibration[0] =
      dt_[0] *
      avgkDO *
      0.5 * (satDo_[0] - outputDo_.dox[0] + satDo_[1]);

   // Loop through time steps
   int lastIndex = length_ - 1;
   for (int i = 1; i < lastIndex; i++) {
      long prevIndex = i - 1;
      outputDo_.dox[i] =
         (
            outputDo_.dox[prevIndex] +
            outputDo_.doProduction[prevIndex] +
            outputDo_.doConsumption[prevIndex] +
            outputDo_.doEquilibration[prevIndex]
         ) /
         (
            1 + (0.5 * (dt_[prevIndex] * avgkDO))
         );

      parDist_[i] = parDistCalculator_.calc(
         dt_[i],
         parAvg_[i]
      );
      output_.cFixation[i] = dailyGPP_ * parDist_[i];
      output_.cRespiration[i] = dailyER_ * dt_[i];

      outputDo_.doProduction[i] =
         output_.cFixation[i] * ratioDoCFix_;
      outputDo_.doConsumption[i] =
         output_.cRespiration[i] * ratioDoCResp_;

      kDo_[i + 1] = kSchmidtDoCalculator_(temp_[i + 1], k600_);
      avgkDO = 0.5 * (kDo_[i] + kDo_[i + 1]);
      outputDo_.doEquilibration[i] =
         dt_[i] *
         avgkDO *
         0.5 * (satDo_[i] - outputDo_.dox[i] + satDo_[i + 1]);
   }

   long prevLastIndex = lastIndex - 1;
   outputDo_.dox[lastIndex] =
      (
         outputDo_.dox[prevLastIndex] +
         outputDo_.doProduction[prevLastIndex] +
         outputDo_.doConsumption[prevLastIndex] +
         outputDo_.doEquilibration[prevLastIndex]
      ) /
      (
         1 + (0.5 * (dt_[prevLastIndex] * avgkDO))
      );

   parDist_[lastIndex] = 0;
   output_.cFixation[lastIndex] = 0;
   output_.cRespiration[lastIndex] = 0;

   outputDo_.doProduction[lastIndex] = 0;
   outputDo_.doConsumption[lastIndex] = 0;
   outputDo_.doEquilibration[lastIndex] = 0;
}
