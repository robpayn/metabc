#include "metabc.h"
#include <cmath>

void MetabForwardEulerDo::run()
{
   // Set the initial oxygen concentration
   outputDo_.dox[0] = initialDO_;

   // Calculate effects on effective organic carbon
   parDist_[0] = parDistCalculator_.calc(
      dt_[0],
      par_[0]
   );
   output_.cFixation[0] = dailyGPP_ * parDist_[0];
   output_.cRespiration[0] = dailyER_ * dt_[0];

   outputDo_.doProduction[0] =
      output_.cFixation[0] * ratioDoCFix_;
   outputDo_.doConsumption[0] =
      output_.cRespiration[0] * ratioDoCResp_;

   kDo_[0] = kSchmidtDoCalculator_(temp_[0], k600_);
   outputDo_.doEquilibration[0] =
      dt_[0] * kDo_[0] * (satDo_[0] - outputDo_.dox[0]);

   // Loop through time steps
   int lastIndex = length_ - 1;
   for (int i = 1; i < lastIndex; i++) {
      long prevIndex = i - 1;
      outputDo_.dox[i] =
         outputDo_.dox[prevIndex] +
         outputDo_.doProduction[prevIndex] +
         outputDo_.doConsumption[prevIndex] +
         outputDo_.doEquilibration[prevIndex];
      if (gwDO_) {
         outputDo_.dox[i] +=
            dt_[prevIndex] * gwAlpha_[prevIndex] *
            (gwDO_[prevIndex] - outputDo_.dox[prevIndex]);
      }

      parDist_[i] = parDistCalculator_.calc(
         dt_[i],
         par_[i]
      );
      output_.cFixation[i] = dailyGPP_ * parDist_[i];
      output_.cRespiration[i] = dailyER_ * dt_[i];

      outputDo_.doProduction[i] =
         output_.cFixation[i] * ratioDoCFix_;
      outputDo_.doConsumption[i] =
         output_.cRespiration[i] * ratioDoCResp_;

      kDo_[i] = kSchmidtDoCalculator_(temp_[i], k600_);
      outputDo_.doEquilibration[i] =
         dt_[i] * kDo_[i] * (satDo_[i] - outputDo_.dox[i]);
   }

   long prevLastIndex = lastIndex - 1;
   outputDo_.dox[lastIndex] =
      outputDo_.dox[prevLastIndex] +
      outputDo_.doProduction[prevLastIndex] +
      outputDo_.doConsumption[prevLastIndex] +
      outputDo_.doEquilibration[prevLastIndex];
   if (gwDO_) {
      outputDo_.dox[lastIndex] +=
         dt_[prevLastIndex] * gwAlpha_[prevLastIndex] *
         (gwDO_[prevLastIndex] - outputDo_.dox[prevLastIndex]);
   }

   parDist_[lastIndex] = 0;
   output_.cFixation[lastIndex] = 0;
   output_.cRespiration[lastIndex] = 0;

   outputDo_.doConsumption[lastIndex] = 0;
   outputDo_.doProduction[lastIndex] = 0;

   kDo_[lastIndex] = kSchmidtDoCalculator_(temp_[lastIndex], k600_);
   outputDo_.doEquilibration[lastIndex] = 0;

}
