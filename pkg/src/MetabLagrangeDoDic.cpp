#include "metabc.h"

MetabLagrangeDoDic::MetabLagrangeDoDic()
{
   setkSchmidtCO2Calculator(kSchmidtCO2Calc);
}

MetabLagrangeDoDic::~MetabLagrangeDoDic()
{
   delete[] upstreamDIC_;
   delete[] pCO2air_;
   delete[] upstreamAlkalinity_;
   delete[] downstreamAlkalinity_;

   delete[] upstreampCO2_;
   delete[] upstreampH_;
   delete[] upstreamSatCO2_;
   delete[] downstreamSatCO2_;
   delete[] upstreamkCO2_;
   delete[] downstreamkCO2_;
   delete[] upstreamkH_;
   delete[] downstreamkH_;

   delete[] outputDic_.pCO2;
   delete[] outputDic_.dic;
   delete[] outputDic_.dicProduction;
   delete[] outputDic_.dicConsumption;
   delete[] outputDic_.co2Equilibration;
   delete[] outputDic_.pH;
}

void MetabLagrangeDoDic::initialize
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
   double ratioDicCFix,
   double ratioDicCResp,
   double* upstreamDIC,
   double* pCO2air,
   double* upstreamAlkalinity,
   double* downstreamAlkalinity
)
{
   MetabLagrangeDo::initialize(
      dailyGPP,
      ratioDoCFix,
      dailyER,
      ratioDoCResp,
      k600,
      upstreamDO,
      upstreamTime,
      downstreamTime,
      upstreamTemp,
      downstreamTemp,
      upstreamPAR,
      downstreamPAR,
      parTotal,
      airPressure,
      stdAirPressure,
      numParcels,
      timeSteps
   );

   upstreamDIC_ = new double[numParcels_];
   pCO2air_ = new double[numParcels_];
   upstreamAlkalinity_ = new double[numParcels_];
   downstreamAlkalinity_ = new double[numParcels_];

   upstreampCO2_ = new double[numParcels_];
   upstreampH_ = new double[numParcels_];
   upstreamSatCO2_ = new double[numParcels_];
   downstreamSatCO2_ = new double[numParcels_];
   upstreamkCO2_ = new double[numParcels_];
   downstreamkCO2_ = new double[numParcels_];
   upstreamkH_ = new double[numParcels_];
   downstreamkH_ = new double[numParcels_];

   outputDic_.pCO2 = new double[numParcels_];
   outputDic_.dic = new double[numParcels_];
   outputDic_.dicProduction = new double[numParcels_];
   outputDic_.dicConsumption = new double[numParcels_];
   outputDic_.co2Equilibration = new double[numParcels_];
   outputDic_.pH = new double[numParcels_];

   // Set the attributes
   ratioDicCFix_ = ratioDicCFix;
   ratioDicCResp_ = ratioDicCResp;

   for(int i = 0; i < numParcels_; i++) {
      // Copy array values into attributes
      upstreamDIC_[i] = upstreamDIC[i];
      pCO2air_[i] = pCO2air[i];
      upstreamAlkalinity_[i] = upstreamAlkalinity[i];
      downstreamAlkalinity_[i] = downstreamAlkalinity[i];

      carbonateEq_.reset(upstreamTemp_[i], 0);
      double equil[2];
      carbonateEq_.optfCO2FromDICTotalAlk(
         upstreamDIC_[i] * 1e-6,
         upstreamAlkalinity_[i] * 1e-6,
         1e-5,
         2,
         12,
         equil
      );
      upstreampH_[i] = equil[0];
      upstreampCO2_[i] = equil[1];
      upstreamkH_[i] = carbonateEq_.kHenryCO2;
      upstreamSatCO2_[i] = upstreamkH_[i] * pCO2air_[i];

      carbonateEq_.reset(downstreamTemp_[i], 0);
      downstreamkH_[i] = carbonateEq_.kHenryCO2;
      downstreamSatCO2_[i] = downstreamkH_[i] * pCO2air_[i];
   }
}

void MetabLagrangeDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator_ = function;
}
