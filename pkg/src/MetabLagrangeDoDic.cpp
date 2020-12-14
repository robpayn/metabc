#include "metabc.h"

MetabLagrangeDoDic::MetabLagrangeDoDic()
{
   setkSchmidtCO2Calculator(kSchmidtCO2Calc);
}

MetabLagrangeDoDic::~MetabLagrangeDoDic()
{
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
   this->upstreamDIC = upstreamDIC;
   this->pCO2air = pCO2air;
   this->upstreamAlkalinity = upstreamAlkalinity;
   this->downstreamAlkalinity = downstreamAlkalinity;

   for(int i = 0; i < numParcels; i++) {
      carbonateEq.reset(upstreamTemp[i], 0);
      double equil[2];
      carbonateEq.optfCO2FromDICTotalAlk(
         upstreamDIC[i] * 1e-6,
         upstreamAlkalinity[i] * 1e-6,
         1e-5,
         2,
         12,
         equil
      );
      upstreampH[i] = equil[0];
      upstreampCO2[i] = equil[1];
      upstreamkH[i] = carbonateEq.kHenryCO2;
      upstreamSatCO2[i] = upstreamkH[i] * pCO2air[i];
      upstreamkCO2[i] = kSchmidtCO2Calculator(upstreamTemp[i], k600);

      carbonateEq.reset(downstreamTemp[i], 0);
      downstreamkH[i] = carbonateEq.kHenryCO2;
      downstreamSatCO2[i] = downstreamkH[i] * pCO2air[i];
      downstreamkCO2[i] = kSchmidtCO2Calculator(downstreamTemp[i], k600);
   }
}

void MetabLagrangeDoDic::setkSchmidtCO2Calculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtCO2Calculator = function;
}
