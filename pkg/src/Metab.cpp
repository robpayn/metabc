#include "metabc.h"
#include <cmath>

Metab::Metab()
{
   // Set the default calculators
   setDensityCalculator(densityWaterCalc);
   setSatDoCalculator(satDoCalc);
   setkSchmidtDoCalculator(kSchmidtDoCalc);
}

Metab::~Metab()
{
   delete[] output_.cFixation;
   delete[] output_.cRespiration;
   delete[] gwAlpha_;
}

void Metab::initialize
(
   double dailyGPP,
   double dailyER,
   double k600,
   int length,
   double* gwAlpha
)
{
   output_.cFixation = new double[length];
   output_.cRespiration = new double[length];

   if (gwAlpha) {
      gwAlpha_ = new double[length];
      for(int i = 0; i < length; i++) {
         gwAlpha_[i] = gwAlpha[i];
      }
   } else {
      gwAlpha_ = nullptr;
   }

   dailyGPP_ = dailyGPP;
   dailyER_ = dailyER;
   k600_ = k600;
   length_ = length;
}

void Metab::setPARDistCalculator(ParDistCalculator calculator)
{
   parDistCalculator_ = calculator;
}

void Metab::setDensityCalculator
(
   double (*function)(double tempC)
)
{
   densityCalculator_ = function;
}

void Metab::setSatDoCalculator
(
   double (*function)(double tempC, double densityWater, double relativePressure)
)
{
   satDoCalculator_ = function;
}

void Metab::setkSchmidtDoCalculator
(
   double (*function)(double tempC, double k600)
)
{
   kSchmidtDoCalculator_ = function;
}

