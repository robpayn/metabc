#include "metabc.h"
#include "utilities.h"

MetabDo::MetabDo
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
   initialize(
      dailyGPP,
      ratioDoCFix,
      dailyER,
      ratioDoCResp,
      k600,
      initialDO,
      time,
      temp,
      par,
      parTotal,
      airPressure,
      stdAirPressure,
      length
   );
}

MetabDo::~MetabDo()
{
   delete[] dt;
   delete[] satDo;
   delete[] kDo;

   delete[] output.cFixation;
   delete[] output.cRespiration;
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
   // Set attributes
   this->dailyGPP = dailyGPP;
   this->ratioDoCFix = ratioDoCFix;
   this->dailyER = dailyER;
   this->ratioDoCResp = ratioDoCResp;
   this->k600 = k600;
   this->initialDO = initialDO;
   this->time = time;
   this->temp = temp;
   this->par = par;
   this->parTotal = parTotal;
   this->airPressure = airPressure;
   this->stdAirPressure = stdAirPressure;
   this->length = length;

   // Set the default calculators
   setDensityCalculator(densityWaterCalc);
   setSatDoCalculator(satDoCalc);
   setkSchmidtDoCalculator(kSchmidtDoCalc);

   // Allocate memory
   dt = new double[length];
   satDo = new double[length];
   kDo = new double[length];

   // Allocate output
   output.cFixation = new double[length];
   output.cRespiration = new double[length];
   outputDo.dox = new double[length];
   outputDo.doProduction = new double[length];
   outputDo.doConsumption = new double[length];
   outputDo.doEquilibration = new double[length];

   // Calculate the change in time between predictions
   //    (forward calculation context)
   for(int i = 0; i < length - 1; i++) {
      dt[i] = time[i + 1] - time[i];
   }
   dt[length - 1] = 0;

   // Calculate DO saturation concentrations and
   //   DO gas exchange rates
   double densityWater;
   for(int i = 0; i < length; i++) {
      densityWater = densityCalculator(temp[i]);
      satDo[i] = satDoCalculator(
         temp[i],
             densityWater,
             airPressure[i] / stdAirPressure
      );
      kDo[i] = kSchmidtDoCalculator(temp[i], k600);
   }
}


void MetabDo::setDensityCalculator
(
   double (*function)(double tempC)
)
{
   densityCalculator = function;
}

void MetabDo::setSatDoCalculator
(
   double (*function)(double tempC, double densityWater, double relativePressure)
)
{
   satDoCalculator = function;
}

void MetabDo::setkSchmidtDoCalculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtDoCalculator = function;
}
