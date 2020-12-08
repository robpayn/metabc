#include <cfloat> /* DBL_EPSILON */
#include <cmath>
#include "carbonate.h"

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

// An approximation of x is returned where an abstract function f(x) attains a minimum on
// the interval (ax,bx).
//
// INPUT..
//    ax
//       left endpoint of initial interval
//    bx
//       right endpoint of initial interval (bx > ax)
//    f
//       function which evaluates  f(x, info)  for any  x
//       in the interval  (ax,bx)
//    info
//       a pointer to any additional information needed by the
//       function f (f must cast the appropriate type to this pointer)
//    tol
//       desired length of the interval of uncertainty of the final
//       result ( >= 0.)
//
// RETURNS..
//    abcissa approximating the point where  f  attains a minimum
//
// The method used is a combination of  golden  section  search  and
// successive parabolic interpolation. Convergence is never much slower
// than  that  for  a  Fibonacci search.  If  f  has a continuous second
// derivative which is positive at the minimum (which is not  at  ax  or
// bx),  then  convergence  is  superlinear, and usually of the order of
// about  1.324....
//
// The function  f  is never evaluated at two points closer together
// than  eps*abs(fmin)+(tol/3), where eps is  approximately  the  square
// root  of  the  relative  machine  precision. If  f  is a unimodal
// function and the computed values of   f   are  always  unimodal  when
// separated  by  at least  eps*abs(x)+(tol/3), then  fmin  approximates
// the abcissa of the global minimum of  f  on the interval  ax,bx  with
// an error less than  3*eps*abs(fmin)+tol.  if   f   is  not  unimodal,
// then fmin may approximate a local, but perhaps non-global, minimum to
// the same accuracy.
//
// This function subprogram is a slightly modified  version  of  the
// Algol  60 procedure  localmin  given in Richard Brent, Algorithms for
// Minimization without Derivatives, Prentice-Hall, Inc. (1973).
//
double Brent_fmin
(
   double ax,
   double bx,
   double (*f)(double, void *),
   void *info,
   double tol
)
{
   /*  c is the squared inverse of the golden ratio */
   const double c = (3. - sqrt(5.)) * .5;

   /* Local variables */
   double a, b, d, e, p, q, r, u, v, w, x;
   double t2, fu, fv, fw, fx, xm, eps, tol1, tol3;

   /*  eps is approximately the square root of the relative machine precision. */
   eps = DBL_EPSILON;
   tol1 = eps + 1.;/* the smallest 1.000... > 1 */
   eps = sqrt(eps);

   a = ax;
   b = bx;
   v = a + c * (b - a);
   w = v;
   x = v;

   d = 0.;/* -Wall */
   e = 0.;
   fx = (*f)(x, info);
   fv = fx;
   fw = fx;
   tol3 = tol / 3.;

   /*  main loop starts here ----------------------------------- */

   for(;;) {
      xm = (a + b) * .5;
      tol1 = eps * fabs(x) + tol3;
      t2 = tol1 * 2.;

      /* check stopping criterion */

      if (fabs(x - xm) <= t2 - (b - a) * .5) break;
      p = 0.;
      q = 0.;
      r = 0.;
      if (fabs(e) > tol1) { /* fit parabola */

      r = (x - w) * (fx - fv);
         q = (x - v) * (fx - fw);
         p = (x - v) * q - (x - w) * r;
         q = (q - r) * 2.;
         if (q > 0.) p = -p; else q = -q;
         r = e;
         e = d;
      }

      if (fabs(p) >= fabs(q * .5 * r) ||
          p <= q * (a - x) || p >= q * (b - x)) { /* a golden-section step */

      if (x < xm) e = b - x; else e = a - x;
      d = c * e;
      }
      else { /* a parabolic-interpolation step */

      d = p / q;
         u = x + d;

         /* f must not be evaluated too close to ax or bx */

         if (u - a < t2 || b - u < t2) {
            d = tol1;
            if (x >= xm) d = -d;
         }
      }

      /* f must not be evaluated too close to x */

      if (fabs(d) >= tol1)
         u = x + d;
      else if (d > 0.)
         u = x + tol1;
      else
         u = x - tol1;

      fu = (*f)(u, info);

      /*  update  a, b, v, w, and x */

      if (fu <= fx) {
         if (u < x) b = x; else a = x;
         v = w;    w = x;   x = u;
         fv = fw; fw = fx; fx = fu;
      } else {
         if (u < x) a = u; else b = u;
         if (fu <= fw || w == x) {
            v = w; fv = fw;
            w = u; fw = fu;
         } else if (fu <= fv || v == x || v == w) {
            v = u; fv = fu;
         }
      }
   }
   /* end of main loop */

   return x;
} // Brent_fmin()

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
