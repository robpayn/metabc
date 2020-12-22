#include "metabc.h"

MetabLagrangeDo::MetabLagrangeDo() :
   Metab()
{}

MetabLagrangeDo::~MetabLagrangeDo()
{
   // Deallocate memory
   delete[] upstreamDO;
   delete[] upstreamTime;
   delete[] downstreamTime;
   delete[] upstreamTemp;
   delete[] downstreamTemp;
   delete[] upstreamPAR;
   delete[] downstreamPAR;
   delete[] airPressure;

   delete[] travelTimes;
   delete[] upstreamSatDo;
   delete[] downstreamSatDo;
   delete[] upstreamkDo;
   delete[] downstreamkDo;
   delete[] parAvg;
   delete[] parDist;

   // Deallocate output
   delete[] outputDo.dox;
   delete[] outputDo.doProduction;
   delete[] outputDo.doConsumption;
   delete[] outputDo.doEquilibration;
}

void MetabLagrangeDo::initialize
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
   int timeSteps
)
{
   Metab::initialize(
     dailyGPP,
     dailyER,
     k600,
     numParcels
   );

   // Allocate memory
   this->upstreamDO = new double[numParcels];
   this->upstreamTime = new double[numParcels];
   this->downstreamTime = new double[numParcels];
   this->upstreamTemp = new double[numParcels];
   this->downstreamTemp = new double[numParcels];
   this->upstreamPAR = new double[numParcels];
   this->downstreamPAR = new double[numParcels];
   this->airPressure = new double[numParcels];

   travelTimes = new double[numParcels];
   upstreamSatDo = new double[numParcels];
   downstreamSatDo = new double[numParcels];
   upstreamkDo = new double[numParcels];
   downstreamkDo = new double[numParcels];
   parAvg = new double[numParcels];
   parDist = new double[numParcels];

   // Allocate output
   outputDo.dox = new double[numParcels];
   outputDo.doProduction = new double[numParcels];
   outputDo.doConsumption = new double[numParcels];
   outputDo.doEquilibration = new double[numParcels];

   // Set the attributes
   this->ratioDoCFix = ratioDoCFix;
   this->ratioDoCResp = ratioDoCResp;
   this->stdAirPressure = stdAirPressure;
   this->numParcels = numParcels;

   // Length of time vector is one larger than the number of time steps
   lengthTimeVector = timeSteps + 1;

   // Calculate values over travel times or at upstream
   // and downstream times
   double densityWater;
   for(int i = 0; i < numParcels; i++) {
      // Copy array values to attributes
      this->upstreamDO[i] = upstreamDO[i];
      this->upstreamTime[i] = upstreamTime[i];
      this->downstreamTime[i] = downstreamTime[i];
      this->upstreamTemp[i] = upstreamTemp[i];
      this->downstreamTemp[i] = downstreamTemp[i];
      this->upstreamPAR[i] = upstreamPAR[i];
      this->downstreamPAR[i] = downstreamPAR[i];
      this->airPressure[i] = airPressure[i];

      // Travel times and average PAR over travel times
      travelTimes[i] = this->downstreamTime[i] - this->upstreamTime[i];
      parAvg[i] = 0.5 * (this->upstreamPAR[i] + this->downstreamPAR[i]);

      // Sat DO and gas exchange rates passing the upstream end
      densityWater = densityCalculator(this->upstreamTemp[i]);
      upstreamSatDo[i] = satDoCalculator(
         this->upstreamTemp[i],
         densityWater,
         this->airPressure[i] / stdAirPressure
      );

      // Sat DO and gas exchange rates passing the downstream end
      densityWater = densityCalculator(this->downstreamTemp[i]);
      downstreamSatDo[i] = satDoCalculator(
         this->downstreamTemp[i],
         densityWater,
         this->airPressure[i] / stdAirPressure
      );
   }

   // Calculate a total par by integration if the
   // total PAR is not provided (i.e. totalPAR argument
   // is <= zero)
   if (parTotal <= 0) {
      double upstreamSum = 0;
      double downstreamSum = 0;
      for(int i = 0; i < numParcels - 1; i++) {
         upstreamSum +=
            (this->upstreamTime[i + 1] - this->upstreamTime[i]) *
            0.5 * (this->upstreamPAR[i] + this->upstreamPAR[i + 1]);
         downstreamSum +=
            (this->downstreamTime[i + 1] - this->downstreamTime[i]) *
            0.5 * (this->downstreamPAR[i] + this->downstreamPAR[i + 1]);
      }
      this->parTotal = 0.5 * (upstreamSum + downstreamSum);
   } else {
      this->parTotal = parTotal;
   }
   parDistCalculator.initialize(this->parTotal);
}
