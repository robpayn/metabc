#include "metabc.h"

MetabLagrangeDoDic::MetabLagrangeDoDic()
{
   setkSchmidtCO2Calculator(kSchmidtCO2Calc);
}

MetabLagrangeDoDic::~MetabLagrangeDoDic()
{
   delete[] upstreamDIC;
   delete[] pCO2air;
   delete[] upstreamAlkalinity;
   delete[] downstreamAlkalinity;

   delete[] upstreampCO2;
   delete[] upstreampH;
   delete[] upstreamSatCO2;
   delete[] downstreamSatCO2;
   delete[] upstreamkCO2;
   delete[] downstreamkCO2;
   delete[] upstreamkH;
   delete[] downstreamkH;

   delete[] outputDic.pCO2;
   delete[] outputDic.dic;
   delete[] outputDic.dicProduction;
   delete[] outputDic.dicConsumption;
   delete[] outputDic.co2Equilibration;
   delete[] outputDic.pH;
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

   this->upstreamDIC = new double[numParcels];
   this->pCO2air = new double[numParcels];
   this->upstreamAlkalinity = new double[numParcels];
   this->downstreamAlkalinity = new double[numParcels];

   upstreampCO2 = new double[numParcels];
   upstreampH = new double[numParcels];
   upstreamSatCO2 = new double[numParcels];
   downstreamSatCO2 = new double[numParcels];
   upstreamkCO2 = new double[numParcels];
   downstreamkCO2 = new double[numParcels];
   upstreamkH = new double[numParcels];
   downstreamkH = new double[numParcels];

   outputDic.pCO2 = new double[numParcels];
   outputDic.dic = new double[numParcels];
   outputDic.dicProduction = new double[numParcels];
   outputDic.dicConsumption = new double[numParcels];
   outputDic.co2Equilibration = new double[numParcels];
   outputDic.pH = new double[numParcels];

   // Set the attributes
   this->ratioDicCFix = ratioDicCFix;
   this->ratioDicCResp = ratioDicCResp;

   for(int i = 0; i < numParcels; i++) {
      // Copy array values into attributes
      this->upstreamDIC[i] = upstreamDIC[i];
      this->pCO2air[i] = pCO2air[i];
      this->upstreamAlkalinity[i] = upstreamAlkalinity[i];
      this->downstreamAlkalinity[i] = downstreamAlkalinity[i];

      carbonateEq.reset(this->upstreamTemp[i], 0);
      double equil[2];
      carbonateEq.optfCO2FromDICTotalAlk(
         this->upstreamDIC[i] * 1e-6,
         this->upstreamAlkalinity[i] * 1e-6,
         1e-5,
         2,
         12,
         equil
      );
      upstreampH[i] = equil[0];
      upstreampCO2[i] = equil[1];
      upstreamkH[i] = carbonateEq.kHenryCO2;
      upstreamSatCO2[i] = upstreamkH[i] * this->pCO2air[i];

      carbonateEq.reset(this->downstreamTemp[i], 0);
      downstreamkH[i] = carbonateEq.kHenryCO2;
      downstreamSatCO2[i] = downstreamkH[i] * this->pCO2air[i];
   }
}

void MetabLagrangeDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator = function;
}
