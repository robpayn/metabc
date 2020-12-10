#include "metabc.h"

template <typename T>
MetabLagrangeDo<T>::MetabLagrangeDo
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
   double* parDist,
   double* airPressure,
   double stdAirPressure,
   int numParcels,
   int timeSteps
)
{
   initialize(
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
      parDist,
      airPressure,
      stdAirPressure,
      numParcels,
      timeSteps
   );
}

template <typename T>
MetabLagrangeDo<T>::~MetabLagrangeDo()
{

}

template <typename T>
void MetabLagrangeDo<T>::initialize
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
   double* parDist,
   double* airPressure,
   double stdAirPressure,
   int numParcels,
   int timeSteps
)
{
   this->dailyGPP = dailyGPP;
   this->ratioDoCFix = ratioDoCFix;
   this->dailyER = dailyER;
   this->ratioDoCResp = ratioDoCResp;
   this->k600 = k600;
   this->upstreamDO = upstreamDO;
   this->upstreamTime = upstreamTime;
   this->downstreamTime = downstreamTime;
   this->upstreamTemp = upstreamTemp;
   this->downstreamTemp = downstreamTemp;
   this->parDist = parDist;
   this->airPressure = airPressure;
   this->stdAirPressure = stdAirPressure;
   this->numParcels = numParcels;

   lengthTimeVector = timeSteps + 1;
}
