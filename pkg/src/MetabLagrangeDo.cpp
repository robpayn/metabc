#include "metabc.h"

MetabLagrangeDo::MetabLagrangeDo() :
   Metab()
{}

MetabLagrangeDo::~MetabLagrangeDo()
{
   // Deallocate memory
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
     ratioDoCFix,
     dailyER,
     ratioDoCResp,
     k600,
     numParcels
   );

   // Allocate memory
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
   this->upstreamDO = upstreamDO;
   this->upstreamTime = upstreamTime;
   this->downstreamTime = downstreamTime;
   this->upstreamTemp = upstreamTemp;
   this->downstreamTemp = downstreamTemp;
   this->upstreamPAR = upstreamPAR;
   this->downstreamPAR = downstreamPAR;
   this->airPressure = airPressure;
   this->stdAirPressure = stdAirPressure;
   this->numParcels = numParcels;

   // Length of time vector is one larger than the number of time steps
   lengthTimeVector = timeSteps + 1;

   // Calculate values over travel times or at upstream
   // and downstream times
   double densityWater;
   for(int i = 0; i < numParcels; i++) {
      // Travel times and average PAR over travel times
      travelTimes[i] = downstreamTime[i] - upstreamTime[i];
      parAvg[i] = 0.5 * (upstreamPAR[i] + downstreamPAR[i]);

      // Sat DO and gas exchange rates passing the upstream end
      densityWater = densityCalculator(upstreamTemp[i]);
      upstreamSatDo[i] = satDoCalculator(
         upstreamTemp[i],
         densityWater,
         airPressure[i] / stdAirPressure
      );
      upstreamkDo[i] = kSchmidtDoCalculator(upstreamTemp[i], k600);

      // Sat DO and gas exchange rates passing the downstream end
      densityWater = densityCalculator(downstreamTemp[i]);
      downstreamSatDo[i] = satDoCalculator(
         downstreamTemp[i],
         densityWater,
         airPressure[i] / stdAirPressure
      );
      downstreamkDo[i] = kSchmidtDoCalculator(downstreamTemp[i], k600);
   }

   // Calculate a total par by integration if the
   // total PAR is not provided (i.e. totalPAR argument
   // is <= zero)
   if (parTotal <= 0) {
      double upstreamSum = 0;
      double downstreamSum = 0;
      for(int i = 0; i < numParcels - 1; i++) {
         // sum +=
         //    (upstreamTime[i + 1] - upstreamTime[i]) *
         //    0.5 * (parAvg[i] + parAvg[i + 1]);
         upstreamSum +=
            (upstreamTime[i + 1] - upstreamTime[i]) *
            0.5 * (upstreamPAR[i] + upstreamPAR[i + 1]);
         downstreamSum +=
            (downstreamTime[i + 1] - downstreamTime[i]) *
            0.5 * (downstreamPAR[i] + downstreamPAR[i + 1]);
      }
      this->parTotal = 0.5 * (upstreamSum + downstreamSum);
   } else {
      this->parTotal = parTotal;
   }
   parDistCalculator.initialize(this->parTotal);
}
