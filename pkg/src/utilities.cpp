#include <cfloat> /* DBL_EPSILON */
#include <cmath>
#include "utilities.h"

ParDistCalculator::ParDistCalculator(double parTotal)
{
   initialize(parTotal);
}

void ParDistCalculator::initialize(double parTotal)
{
   this->parTotal = parTotal;
}

double ParDistCalculator::calc
(
   double duration,
   double par
)
{
   return (par * duration) / parTotal;
}

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
