#include <R.h>
#include <Rinternals.h>
#include "carbonate.h"

extern "C"
{
   SEXP CarbonateEq_constructor(SEXP tempC, SEXP eConduct);
   SEXP CarbonateEq_reset(SEXP externalPointer, SEXP tempC, SEXP eConduct);
   SEXP CarbonateEq_optfCO2FromDICTotalAlk(
      SEXP externalPointer,
      SEXP concDIC,
      SEXP totalAlk,
      SEXP pHtolerance,
      SEXP pHmin,
      SEXP pHmax
   );
}
