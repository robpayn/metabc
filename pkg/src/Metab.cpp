#include "metabc.h"

Metab::Metab()
{
   // Set the default calculators
   setDensityCalculator(densityWaterCalc);
   setSatDoCalculator(satDoCalc);
   setkSchmidtDoCalculator(kSchmidtDoCalc);
}

Metab::~Metab()
{
   delete[] output.cFixation;
   delete[] output.cRespiration;
}

void Metab::initialize
(
   double dailyGPP,
   double ratioDoCFix,
   double dailyER,
   double ratioDoCResp,
   double k600,
   int length
)
{
   output.cFixation = new double[length];
   output.cRespiration = new double[length];

   this->dailyGPP = dailyGPP;
   this->ratioDoCFix = ratioDoCFix;
   this->dailyER = dailyER;
   this->ratioDoCResp = ratioDoCResp;
   this->k600 = k600;
   this->length = length;
}

void Metab::setPARDistCalculator(ParDistCalculator calculator)
{
   parDistCalculator = calculator;
}

void Metab::setDensityCalculator
(
   double (*function)(double tempC)
)
{
   densityCalculator = function;
}

void Metab::setSatDoCalculator
(
   double (*function)(double tempC, double densityWater, double relativePressure)
)
{
   satDoCalculator = function;
}

void Metab::setkSchmidtDoCalculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtDoCalculator = function;
}

