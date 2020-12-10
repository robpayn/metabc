#include "metabc_R.h"

SEXP MetabCrankNicolsonDo_constructor(
   SEXP dailyGPP,
   SEXP ratioDoCFix,
   SEXP dailyER,
   SEXP ratioDoCResp,
   SEXP k600,
   SEXP initialDO,
   SEXP time,
   SEXP temp,
   SEXP parDist,
   SEXP airPressure,
   SEXP stdAirPressure
)
{
   int length = length(time);

   MetabCrankNicolsonDo* modelPointer = new MetabCrankNicolsonDo(
      asReal(dailyGPP),
      asReal(ratioDoCFix),
      asReal(dailyER),
      asReal(ratioDoCResp),
      asReal(k600),
      asReal(initialDO),
      REAL(time),
      REAL(temp),
      REAL(parDist),
      REAL(airPressure),
      asReal(stdAirPressure),
      length
   );
   SEXP modelExtPointer = PROTECT(
      R_MakeExternalPtr(modelPointer, R_NilValue, R_NilValue)
   );

   R_RegisterCFinalizer(
      modelExtPointer,
      finalizerExternalPointer<MetabCrankNicolsonDo>
   );

   MetabDo* basePointer = dynamic_cast <MetabDo*> (modelPointer);
   SEXP baseExtPointer = PROTECT(
      R_MakeExternalPtr(basePointer, R_NilValue, R_NilValue)
   );

   SEXP vecOutput = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vecOutput, 0, modelExtPointer);
   SET_VECTOR_ELT(vecOutput, 1, baseExtPointer);

   SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vecOutput_names, 0, install("modelExternalPointer"));
   SET_VECTOR_ELT(vecOutput_names, 1, install("baseExternalPointer"));

   setAttrib(vecOutput, install("names"), vecOutput_names);

   UNPROTECT(4);
   return vecOutput;
}

SEXP MetabCrankNicolsonDo_destructor(SEXP modelExtPointer)
{
   finalizerExternalPointer<MetabCrankNicolsonDo>(modelExtPointer);

   return R_NilValue;
}
