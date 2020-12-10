#include "metabc.h"
#include "utilities.h"

MetabDoDic::MetabDoDic
(
   double dailyGPP,
   double ratioDoCFix,
   double dailyER,
   double ratioDoCResp,
   double k600,
   double initialDO,
   double* time,
   double* temp,
   double* parDist,
   double* airPressure,
   double stdAirPressure,
   int length,
   double ratioDicCfix,
   double ratioDicCresp,
   double initialDIC,
   double* pCO2air,
   double* alkalinity
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
      parDist,
      airPressure,
      stdAirPressure,
      length,
      ratioDicCfix,
      ratioDicCresp,
      initialDIC,
      pCO2air,
      alkalinity
   );
}

MetabDoDic::~MetabDoDic()
{
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
   double* parDist,
   double* airPressure,
   double stdAirPressure,
   int length,
   double ratioDicCfix,
   double ratioDicCresp,
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
      parDist,
      airPressure,
      stdAirPressure,
      length
   );

   this->ratioDicCfix = ratioDicCfix;
   this->ratioDicCresp = ratioDicCresp;
   this->initialDIC = initialDIC;
   this->pCO2air = pCO2air;
   this->alkalinity = alkalinity;

   carbonateEq.reset(temp[0], 0);

   setkSchmidtCO2Calculator(kSchmidtCO2Calc);

   kCO2 = new double[length];
   kH = new double[length];

   outputDic.pCO2 = new double[length];
   outputDic.dic = new double[length];
   outputDic.dicProduction = new double[length];
   outputDic.dicConsumption = new double[length];
   outputDic.co2Equilibration = new double[length];
   outputDic.pH = new double[length];

   for(int i = 0; i < length; i++) {
      kCO2[i] = kSchmidtCO2Calculator(temp[i], k600);
   }
}

void MetabDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator = function;
}
