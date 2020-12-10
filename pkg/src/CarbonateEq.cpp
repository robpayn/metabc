#include <cmath>
#include "carbonate.h"
#include "utilities.h"

struct proposepH_info
{
   CarbonateEq* carbonateEq;
   double totalAlk;
   double concDIC;
};

double proposepH(double pH, void* info)
{
   proposepH_info* p = (proposepH_info*)info;
   double totalAlkFromDICpH =
      p->carbonateEq->calcTotalAlkFromDICpH(p->concDIC, pH);

   return fabs(p->totalAlk - totalAlkFromDICpH);
}

CarbonateEq::CarbonateEq
(
   double tempC,
   double eConduct
)
{
   reset(tempC, eConduct);
}

void CarbonateEq::reset
(
   double tempC,
   double eConduct
)
{
   this->tempC = tempC;
   this->eConduct = eConduct;
   tempK = tempC + 273.15;
   ionicStrength = 0.013 * eConduct;
   daviesParam = 0.5092 + (tempC - 25) * 0.00085;
   daviesExponent =
      -daviesParam *
      (
         (
            pow(ionicStrength, 0.5) /
            (1 + pow(ionicStrength, 0.5))
         ) -
         0.3 * ionicStrength
      );

   // Activitity coefficients from Davies equation
   // based on square of ionic charge

   // Charge of -1 or +1 so exponent multiplier is 1
   activityCoeffH = pow(10, daviesExponent);
   activityCoeffOH = activityCoeffH;
   activityCoeffHCO3 = activityCoeffH;
   // Charge of -2 so exponent multiplier is 4
   activityCoeffCO3 = pow(10, 4 * daviesExponent);

   double logTempK = log(tempK);

   // Ideal and apparent dissociation constant for H2CO3
   // (Millero 1979 - Geochimica et Cosmochimica Acta)
   double kDissocH2CO3 = exp(
      290.9097 -
      (14554.21 / tempK) -
      (45.0575 * logTempK)
   );
   kDissocH2CO3App =
      kDissocH2CO3 /
      (activityCoeffH * activityCoeffHCO3);

   // Ideal and apparent dissociation constant for HCO3
   // (Millero 1979 - Geochimica et Cosmochimica Acta)
   double kDissocHCO3 = exp(
      207.6548 -
      (11843.79 / tempK) -
      (33.6485 * logTempK)
   );
   kDissocHCO3App =
      kDissocHCO3 /
      (
         activityCoeffH *
         (activityCoeffCO3 / activityCoeffHCO3)
      );

   // Ideal and apparent dissociation constant for freshwater
   // (Millero 1995 - Geochimica et Cosmochimica Acta)
   double kDissocH2O = exp(
      (-13847.26 / tempK) +
      148.9802 -
      (23.6521 * logTempK)
   );
   kDissocH2OApp =
      kDissocH2O /
      (activityCoeffH * activityCoeffOH);

   kHenryCO2 = exp(
      -60.2409 +
      (93.4517 * (100 / tempK))  +
      (23.3585 * log(tempK / 100))
   );
}

void CarbonateEq::optfCO2FromDICTotalAlk
(
   double concDIC,
   double totalAlk,
   double pHtolerance,
   double pHmin,
   double pHmax,
   double out[]
)
{
   out[0] = optpHFromDICTotalAlk(
      concDIC,
      totalAlk,
      pHtolerance,
      pHmin,
      pHmax
   );
   out[1] = calcfCO2FromDICpH(concDIC, out[0]);
}

double CarbonateEq::optpHFromDICTotalAlk
(
   double concDIC,
   double totalAlk,
   double tolerance,
   double min,
   double max
)
{
   proposepH_info info;
   info.carbonateEq = this;
   info.totalAlk = totalAlk;
   info.concDIC = concDIC;

   double pH = Brent_fmin(
      min,
      max,
      proposepH,
      &info,
      tolerance
   );

   return pH;
}

double CarbonateEq::calcTotalAlkFromDICpH
(
   double concDIC,
   double pH
)
{
   double concH = pow(10, -pH);
   double concOH = kDissocH2OApp / concH;
   double concCO2 =
      concDIC *
      pow(concH, 2) /
      (
         pow(concH , 2) +
         kDissocH2CO3App * concH +
         kDissocH2CO3App * kDissocHCO3App
      );
   double concHCO3 =
      concCO2 *
      kDissocH2CO3App / concH;
   double concCO3 =
      concHCO3 *
      kDissocHCO3App / concH;
   return concHCO3 + 2 * concCO3 + concOH - concH;
}

double CarbonateEq::calcfCO2FromDICpH
(
   double concDIC,
   double pH
)
{
   double concH = pow(10, -pH);
   double concCO2 =
      concDIC *
      pow(concH, 2) *
      pow(10, 6 * daviesExponent) /
      (
         pow(concH, 2) * pow(10, 6 * daviesExponent) +
         kDissocH2CO3App * concH * pow(10, 4 * daviesExponent) +
         kDissocH2CO3App * kDissocHCO3App
      );
   return 1e6 * (concCO2 / kHenryCO2);
}
