#include <cmath>
#include "utilities.h"

double densityWaterCalc(double tempC)
{
   return
      0.999842 +
      6.7940e-5 * tempC -
      9.0953e-6 * pow(tempC, 2) +
      1.0017e-7 * pow(tempC, 3) -
      1.1201e-9 * pow(tempC, 4) +
      6.5363e-12 * pow(tempC, 5);
}

double satDoCalc(double tempC, double densityWater, double relativePressure)
{
   double normTemp = log(
      (298.15 - tempC) /
      (273.15 + tempC)
   );
   return
      relativePressure *
      densityWater *
      exp(
         5.80871 +
         3.20291 * normTemp +
         4.17887 * pow(normTemp, 2) +
         5.10006 * pow(normTemp, 3) -
         0.0986643 * pow(normTemp, 4) +
         3.80369 * pow(normTemp, 5)
      );
}

double kSchmidtDoCalc(double tempC, double k600)
{
   double schmidt = (
      1800.6 -
      120.1 * tempC +
      3.7818 * pow(tempC, 2) -
      0.047608 * pow(tempC, 3)
   );
   return k600 * pow((schmidt / 600), -0.5);
}

double kSchmidtCO2Calc(double tempC, double k600)
{
   double schmidt =
      1742 -
      91.24 * tempC +
      2.208 * pow(tempC, 2) -
      0.0219 * pow(tempC, 3);
   return k600 * pow((schmidt / 600), -0.5);
}
