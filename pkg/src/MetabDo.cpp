#include "metabc.h"

MetabDo::MetabDo() :
   Metab()
{}

MetabDo::~MetabDo()
{
   delete[] time;
   delete[] temp;
   delete[] par;
   delete[] airPressure;

   delete[] dt;
   delete[] satDo;
   delete[] kDo;
   delete[] parAvg;
   delete[] parDist;

   delete[] outputDo.dox;
   delete[] outputDo.doProduction;
   delete[] outputDo.doConsumption;
   delete[] outputDo.doEquilibration;
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
   int length
)
{
   // Initialize the base class
   Metab::initialize(
      dailyGPP,
      dailyER,
      k600,
      length
   );

   // Allocate memory
   this->time = new double[length];
   this->temp = new double[length];
   this->par = new double[length];
   this->airPressure = new double[length];

   dt = new double[length];
   satDo = new double[length];
   kDo = new double[length];
   parAvg = new double[length];
   parDist = new double[length];

   // Allocate output
   outputDo.dox = new double[length];
   outputDo.doProduction = new double[length];
   outputDo.doConsumption = new double[length];
   outputDo.doEquilibration = new double[length];

   // Set attributes
   this->ratioDoCFix = ratioDoCFix;
   this->ratioDoCResp = ratioDoCResp;
   this->initialDO = initialDO;
   this->stdAirPressure = stdAirPressure;

   for(int i = 0; i < length; i++) {
      this->time[i] = time[i];
      this->temp[i] = temp[i];
      this->par[i] = par[i];
      this->airPressure[i] = airPressure[i];
   }

   int lastIndex = length - 1;

   // Calculate the values at times or over time steps
   double densityWater;
   for(int i = 0; i < lastIndex; i++) {
      dt[i] = this->time[i + 1] - this->time[i];
      parAvg[i] = 0.5 * (this->par[i] + this->par[i + 1]);

      densityWater = densityCalculator(this->temp[i]);
      satDo[i] = satDoCalculator(
         this->temp[i],
         densityWater,
         this->airPressure[i] / stdAirPressure
      );
   }
   dt[lastIndex] = 0;
   parAvg[lastIndex] = 0;

   densityWater = densityCalculator(this->temp[lastIndex]);
   satDo[lastIndex] = satDoCalculator(
      this->temp[lastIndex],
      densityWater,
      this->airPressure[lastIndex] / stdAirPressure
   );

   // Calculate a total par by integration if the
   // total PAR is not provided (i.e. totalPAR argument
   // is <= zero)
   if (parTotal <= 0) {
      double sum = 0;
      for(int i = 0; i < lastIndex; i++) {
         sum += parAvg[i] * dt[i];
      }
      this->parTotal = sum;
   } else {
      this->parTotal = parTotal;
   }
   parDistCalculator.initialize(this->parTotal);

}
