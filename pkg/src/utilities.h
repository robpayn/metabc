
class ParDistCalculator {
   public:
      ParDistCalculator(){};
      ParDistCalculator(double parTotal);
      double parTotal;
      void initialize(double parTotal);
      double calc
      (
         double duration,
         double par
      );
};

//!  Calculates the density of water
/*!
 *   Density of water is calculated based on an empirical
 *   relationship with the temperature of water
 *
 *   \param temp
 *     Numerical vector of temperatures (deg C)
 *
 *   \return
 *     Numerical vector of densities of water in kilograms per liter
 */
double densityWaterCalc(double tempC);

double satDoCalc(
   double tempC,
   double densityWater,
   double relativePressure
);

double kSchmidtDoCalc(double tempC, double k600);

double kSchmidtCO2Calc(double tempC, double k600);

double Brent_fmin(
   double ax,
   double bx,
   double (*f)(double, void *),
   void *info,
   double tol
);
