double densityWaterCalc(double tempC);

double satDoCalc(double tempC, double densityWater, double relativePressure);

double kSchmidtDoCalc(double tempC, double k600);

double kSchmidtCO2Calc(double tempC, double k600);

double Brent_fmin(
      double ax,
      double bx,
      double (*f)(double, void *),
      void *info,
      double tol
);
