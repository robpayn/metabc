#include "metabc.h"
#include <cmath>

void MetabLagrangeCNOneStepDo::run()
{
   // Loop through time steps
   for (int i = 0; i < numParcels_; i++) {
      parDist_[i] = parDistCalculator_.calc(
         travelTimes_[i],
         parAvg_[i]
      );
      output_.cFixation[i] = dailyGPP_ * parDist_[i];
      output_.cRespiration[i] = dailyER_ * travelTimes_[i];

      outputDo_.doProduction[i] =
         output_.cFixation[i] * ratioDoCFix_;
      outputDo_.doConsumption[i] =
         output_.cRespiration[i] * ratioDoCResp_;

      upstreamkDo_[i] = kSchmidtDoCalculator_(upstreamTemp_[i], k600_);
      downstreamkDo_[i] = kSchmidtDoCalculator_(downstreamTemp_[i], k600_);
      double avgkDO = 0.5 * (upstreamkDo_[i] + downstreamkDo_[i]);
      outputDo_.doEquilibration[i] =
         travelTimes_[i] *
         avgkDO *
         0.5 * (upstreamSatDo_[i] - upstreamDO_[i] + downstreamSatDo_[i]);

      double numerator =
         upstreamDO_[i] +
         outputDo_.doProduction[i] +
         outputDo_.doConsumption[i] +
         outputDo_.doEquilibration[i];
      double denominator =
         1 + (0.5 * (travelTimes_[i] * avgkDO));

      if (gwDO_) {
         numerator += travelTimes_[i] * gwAlpha_[i] *
            (gwDO_[i] - (0.5 * upstreamDO_[i]));
         denominator += 0.5 * (travelTimes_[i] * gwAlpha_[i]);
      }

      outputDo_.dox[i] = numerator / denominator;
   }

}
