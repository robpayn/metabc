#include "carbonate_R.h"

void finalizerCarbonateEq(SEXP externalPointer) {

   CarbonateEq* pointer = (CarbonateEq*)R_ExternalPtrAddr(externalPointer);
   R_ClearExternalPtr(externalPointer);
   delete pointer;

}

SEXP CarbonateEq_destructor(SEXP externalPointer)
{
   finalizerCarbonateEq(externalPointer);

   return R_NilValue;
}

SEXP CarbonateEq_constructor(SEXP tempC, SEXP eConduct)
{
   CarbonateEq* pointer = dynamic_cast <CarbonateEq*> (
      new CarbonateEq(asReal(tempC), asReal(eConduct))
   );

   SEXP externalPointer = PROTECT(
      R_MakeExternalPtr(pointer, R_NilValue, R_NilValue)
   );

   R_RegisterCFinalizer(
      externalPointer,
      finalizerCarbonateEq
   );

   UNPROTECT(1);

   return externalPointer;
}

SEXP CarbonateEq_reset(SEXP externalPointer, SEXP tempC, SEXP eConduct)
{
   CarbonateEq* pointer = (CarbonateEq*)R_ExternalPtrAddr(externalPointer);

   pointer->reset(asReal(tempC), asReal(eConduct));

   return R_NilValue;
}

SEXP CarbonateEq_optfCO2FromDICTotalAlk(
      SEXP externalPointer,
      SEXP concDIC,
      SEXP totalAlk,
      SEXP pHtolerance,
      SEXP pHmin,
      SEXP pHmax
   )
{
   CarbonateEq* pointer = (CarbonateEq*)R_ExternalPtrAddr(externalPointer);

   double out[2];
   pointer->optfCO2FromDICTotalAlk(
      asReal(concDIC),
      asReal(totalAlk),
      asReal(pHtolerance),
      asReal(pHmin),
      asReal(pHmax),
      out
   );

   SEXP ret = PROTECT(allocVector(REALSXP, 2));
   REAL(ret)[0] = out[0];
   REAL(ret)[1] = out[1];

   SEXP ret_names = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(ret_names, 0, install("pH"));
   SET_VECTOR_ELT(ret_names, 1, install("pCO2"));

   setAttrib(ret, install("names"), ret_names);

   UNPROTECT(2);

   return ret;
}
