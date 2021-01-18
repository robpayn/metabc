#include "metabc.h"

MetabLagrangeDo::MetabLagrangeDo() :
   Metab()
{}

MetabLagrangeDo::~MetabLagrangeDo()
{
   // Deallocate memory
   delete[] upstreamDO_;
   delete[] upstreamTime_;
   delete[] downstreamTime_;
   delete[] upstreamTemp_;
   delete[] downstreamTemp_;
   delete[] upstreamPAR_;
   delete[] downstreamPAR_;
   delete[] airPressure_;
   delete[] gwDO_;

   delete[] travelTimes_;
   delete[] upstreamSatDo_;
   delete[] downstreamSatDo_;
   delete[] upstreamkDo_;
   delete[] downstreamkDo_;
   delete[] parAvg_;
   delete[] parDist_;

   // Deallocate output
   delete[] outputDo_.dox;
   delete[] outputDo_.doProduction;
   delete[] outputDo_.doConsumption;
   delete[] outputDo_.doEquilibration;
}

void MetabLagrangeDo::initialize
(
   double dailyGPP,
   double ratioDoCFix,
   double dailyER,
   double ratioDoCResp,
   double k600,
   double* upstreamDO,
   double* upstreamTime,
   double* downstreamTime,
   double* upstreamTemp,
   double* downstreamTemp,
   double* upstreamPAR,
   double* downstreamPAR,
   double parTotal,
   double* airPressure,
   double stdAirPressure,
   int numParcels,
   int timeSteps,
   double* gwAlpha,
   double* gwDO
)
{
   Metab::initialize(
     dailyGPP,
     dailyER,
     k600,
     numParcels,
     gwAlpha
   );

   numParcels_ = numParcels;

   // Allocate memory
   upstreamDO_ = new double[numParcels_];
   upstreamTime_ = new double[numParcels_];
   downstreamTime_ = new double[numParcels_];
   upstreamTemp_ = new double[numParcels_];
   downstreamTemp_ = new double[numParcels_];
   upstreamPAR_ = new double[numParcels_];
   downstreamPAR_ = new double[numParcels_];
   airPressure_ = new double[numParcels_];

   if (gwAlpha_ && gwDO) {
      gwDO_ = new double[numParcels_];
      for(int i = 0; i < numParcels_; i++) {
         gwDO_[i] = gwDO[i];
      }
   } else {
      gwDO_ = nullptr;
   }

   travelTimes_ = new double[numParcels_];
   upstreamSatDo_ = new double[numParcels_];
   downstreamSatDo_ = new double[numParcels_];
   upstreamkDo_ = new double[numParcels_];
   downstreamkDo_ = new double[numParcels_];
   parAvg_ = new double[numParcels_];
   parDist_ = new double[numParcels_];

   // Allocate output
   outputDo_.dox = new double[numParcels_];
   outputDo_.doProduction = new double[numParcels_];
   outputDo_.doConsumption = new double[numParcels_];
   outputDo_.doEquilibration = new double[numParcels_];

   // Set the attributes
   ratioDoCFix_ = ratioDoCFix;
   ratioDoCResp_ = ratioDoCResp;
   stdAirPressure_ = stdAirPressure;

   // Length of time vector is one larger than the number of time steps
   lengthTimeVector_ = timeSteps + 1;

   // Calculate values over travel times or at upstream
   // and downstream times
   double densityWater;
   for(int i = 0; i < numParcels_; i++) {
      // Copy array values to attributes
      upstreamDO_[i] = upstreamDO[i];
      upstreamTime_[i] = upstreamTime[i];
      downstreamTime_[i] = downstreamTime[i];
      upstreamTemp_[i] = upstreamTemp[i];
      downstreamTemp_[i] = downstreamTemp[i];
      upstreamPAR_[i] = upstreamPAR[i];
      downstreamPAR_[i] = downstreamPAR[i];
      airPressure_[i] = airPressure[i];

      // Travel times and average PAR over travel times
      travelTimes_[i] = downstreamTime_[i] - upstreamTime_[i];
      parAvg_[i] = 0.5 * (upstreamPAR_[i] + downstreamPAR_[i]);

      // Sat DO and gas exchange rates passing the upstream end
      densityWater = densityCalculator_(upstreamTemp_[i]);
      upstreamSatDo_[i] = satDoCalculator_(
         upstreamTemp_[i],
         densityWater,
         airPressure_[i] / stdAirPressure
      );

      // Sat DO and gas exchange rates passing the downstream end
      densityWater = densityCalculator_(downstreamTemp_[i]);
      downstreamSatDo_[i] = satDoCalculator_(
         downstreamTemp_[i],
         densityWater,
         airPressure_[i] / stdAirPressure
      );
   }

   // Calculate a total par by integration if the
   // total PAR is not provided (i.e. totalPAR argument
   // is <= zero)
   if (parTotal <= 0) {
      double upstreamSum = 0;
      double downstreamSum = 0;
      for(int i = 0; i < numParcels_ - 1; i++) {
         upstreamSum +=
            (upstreamTime_[i + 1] - upstreamTime_[i]) *
            0.5 * (upstreamPAR_[i] + upstreamPAR_[i + 1]);
         downstreamSum +=
            (downstreamTime_[i + 1] - downstreamTime_[i]) *
            0.5 * (downstreamPAR_[i] + downstreamPAR_[i + 1]);
      }
      parTotal_ = 0.5 * (upstreamSum + downstreamSum);
   } else {
      parTotal_ = parTotal;
   }
   parDistCalculator_.initialize(parTotal_);
}
