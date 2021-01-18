#include "metabc.h"

MetabDo::MetabDo() :
   Metab()
{}

MetabDo::~MetabDo()
{
   delete[] time_;
   delete[] temp_;
   delete[] par_;
   delete[] airPressure_;
   delete[] gwDO_;

   delete[] dt_;
   delete[] satDo_;
   delete[] kDo_;
   delete[] parAvg_;
   delete[] parDist_;

   delete[] outputDo_.dox;
   delete[] outputDo_.doProduction;
   delete[] outputDo_.doConsumption;
   delete[] outputDo_.doEquilibration;
}

void MetabDo::initialize
(
   double dailyGPP,
   double ratioDoCFix,
   double dailyER,
   double ratioDoCResp,
   double k600,
   double initialDO,
   double* time,
   double* temp,
   double* par,
   double parTotal,
   double* airPressure,
   double stdAirPressure,
   int length,
   double* gwAlpha,
   double* gwDO
)
{
   // Initialize the base class
   Metab::initialize(
      dailyGPP,
      dailyER,
      k600,
      length,
      gwAlpha
   );

   // Allocate memory
   time_ = new double[length_];
   temp_ = new double[length_];
   par_ = new double[length_];
   airPressure_ = new double[length_];

   if (gwAlpha_ && gwDO) {
      gwDO_ = new double[length_];
      for(int i = 0; i < length_; i++) {
         gwDO_[i] = gwDO[i];
      }
   } else {
      gwDO_ = nullptr;
   }

   dt_ = new double[length_];
   satDo_ = new double[length_];
   kDo_ = new double[length_];
   parAvg_ = new double[length_];
   parDist_ = new double[length_];

   // Allocate output
   outputDo_.dox = new double[length_];
   outputDo_.doProduction = new double[length_];
   outputDo_.doConsumption = new double[length_];
   outputDo_.doEquilibration = new double[length_];

   // Set attributes
   ratioDoCFix_ = ratioDoCFix;
   ratioDoCResp_ = ratioDoCResp;
   initialDO_ = initialDO;
   stdAirPressure_ = stdAirPressure;

   for(int i = 0; i < length_; i++) {
      time_[i] = time[i];
      temp_[i] = temp[i];
      par_[i] = par[i];
      airPressure_[i] = airPressure[i];
   }

   int lastIndex = length_ - 1;

   // Calculate the values at times or over time steps
   double densityWater;
   for(int i = 0; i < lastIndex; i++) {
      dt_[i] = time_[i + 1] - time_[i];
      parAvg_[i] = 0.5 * (par_[i] + par_[i + 1]);

      densityWater = densityCalculator_(temp_[i]);
      satDo_[i] = satDoCalculator_(
         temp_[i],
         densityWater,
         airPressure_[i] / stdAirPressure
      );
   }
   dt_[lastIndex] = 0;
   parAvg_[lastIndex] = 0;

   densityWater = densityCalculator_(temp_[lastIndex]);
   satDo_[lastIndex] = satDoCalculator_(
      temp_[lastIndex],
      densityWater,
      airPressure_[lastIndex] / stdAirPressure
   );

   // Calculate a total par by integration if the
   // total PAR is not provided (i.e. totalPAR argument
   // is <= zero)
   if (parTotal <= 0) {
      double sum = 0;
      for(int i = 0; i < lastIndex; i++) {
         sum += parAvg_[i] * dt_[i];
      }
      parTotal_ = sum;
   } else {
      parTotal_ = parTotal;
   }
   parDistCalculator_.initialize(parTotal_);

}
