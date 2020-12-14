#include "metabc.h"

MetabDo::MetabDo() :
   Metab()
{}

MetabDo::~MetabDo()
{
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
      ratioDoCFix,
      dailyER,
      ratioDoCResp,
      k600,
      length
   );

   // Allocate memory
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
   this->initialDO = initialDO;
   this->time = time;
   this->temp = temp;
   this->par = par;
   this->airPressure = airPressure;
   this->stdAirPressure = stdAirPressure;

   int lastIndex = length - 1;

   // Calculate the values at times or over time steps
   double densityWater;
   for(int i = 0; i < lastIndex; i++) {
      dt[i] = time[i + 1] - time[i];
      parAvg[i] = 0.5 * (par[i] + par[i + 1]);

      densityWater = densityCalculator(temp[i]);
      satDo[i] = satDoCalculator(
         temp[i],
         densityWater,
         airPressure[i] / stdAirPressure
      );

      kDo[i] = kSchmidtDoCalculator(temp[i], k600);
   }
   dt[lastIndex] = 0;
   parAvg[lastIndex] = 0;

   densityWater = densityCalculator(temp[lastIndex]);
   satDo[lastIndex] = satDoCalculator(
      temp[lastIndex],
      densityWater,
      airPressure[lastIndex] / stdAirPressure
   );

   kDo[lastIndex] = kSchmidtDoCalculator(temp[lastIndex], k600);

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
