#include "metabc.h"

MetabDoDic::MetabDoDic()
{
   setkSchmidtCO2Calculator(kSchmidtCO2Calc);
}

MetabDoDic::~MetabDoDic()
{
   delete[] pCO2air;
   delete[] alkalinity;

   delete[] kCO2;
   delete[] kH;

   delete[] outputDic.pCO2;
   delete[] outputDic.dic;
   delete[] outputDic.dicProduction;
   delete[] outputDic.dicConsumption;
   delete[] outputDic.co2Equilibration;
   delete[] outputDic.pH;
}

void MetabDoDic::initialize
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
   double ratioDicCFix,
   double ratioDicCResp,
   double initialDIC,
   double* pCO2air,
   double* alkalinity
)
{
   MetabDo::initialize(
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

   // Allocate memory for array attributes
   this->pCO2air = new double[length];
   this->alkalinity = new double[length];

   kCO2 = new double[length];
   kH = new double[length];

   outputDic.pCO2 = new double[length];
   outputDic.dic = new double[length];
   outputDic.dicProduction = new double[length];
   outputDic.dicConsumption = new double[length];
   outputDic.co2Equilibration = new double[length];
   outputDic.pH = new double[length];

   // Assign attribute values
   this->ratioDicCFix = ratioDicCFix;
   this->ratioDicCResp = ratioDicCResp;
   this->initialDIC = initialDIC;

   // Copy array values into attributes
   for(int i = 0; i < length; i++) {
      this->pCO2air[i] = pCO2air[i];
      this->alkalinity[i] = alkalinity[i];
   }

   carbonateEq.reset(this->temp[0], 0);
}

void MetabDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator = function;
}
