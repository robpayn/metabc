#include "metabc.h"

MetabDoDic::MetabDoDic()
{
   setkSchmidtCO2Calculator(kSchmidtCO2Calc);
}

MetabDoDic::~MetabDoDic()
{
   delete[] pCO2air_;
   delete[] alkalinity_;
   delete[] gwDIC_;

   delete[] kCO2_;
   delete[] kH_;

   delete[] outputDic_.pCO2;
   delete[] outputDic_.dic;
   delete[] outputDic_.dicProduction;
   delete[] outputDic_.dicConsumption;
   delete[] outputDic_.co2Equilibration;
   delete[] outputDic_.pH;
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
   double* alkalinity,
   double* gwAlpha,
   double* gwDO,
   double* gwDIC
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
      length,
      gwAlpha,
      gwDO
   );

   // Allocate memory for array attributes
   pCO2air_ = new double[length_];
   alkalinity_ = new double[length_];

   if (gwAlpha_ && gwDIC) {
      gwDIC_ = new double[length_];
      for(int i = 0; i < length_; i++) {
         gwDIC_[i] = gwDIC[i];
      }
   } else {
      gwDIC_ = nullptr;
   }

   kCO2_ = new double[length_];
   kH_ = new double[length_];

   outputDic_.pCO2 = new double[length_];
   outputDic_.dic = new double[length_];
   outputDic_.dicProduction = new double[length_];
   outputDic_.dicConsumption = new double[length_];
   outputDic_.co2Equilibration = new double[length_];
   outputDic_.pH = new double[length_];

   // Assign attribute values
   ratioDicCFix_ = ratioDicCFix;
   ratioDicCResp_ = ratioDicCResp;
   initialDIC_ = initialDIC;

   // Copy array values into attributes
   for(int i = 0; i < length_; i++) {
      pCO2air_[i] = pCO2air[i];
      alkalinity_[i] = alkalinity[i];
   }

   carbonateEq_.reset(temp_[0], 0);
}

void MetabDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator_ = function;
}
