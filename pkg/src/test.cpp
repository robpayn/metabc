#include <R.h>
#include <Rinternals.h>
#include <cmath>
#include "carbonate.h"

extern "C" {
   SEXP CarbonateEq_constructor(SEXP temp, SEXP econduct)
   {
      CarbonateEq* pointer = new CarbonateEq(asReal(temp), asReal(econduct));
      SEXP extPointer = PROTECT(
         R_MakeExternalPtr(pointer, R_NilValue, R_NilValue)
      );

      UNPROTECT(1);
      return extPointer;
   }
}
