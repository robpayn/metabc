#include "metabc_R.h"

extern "C" {

   SEXP MetabCrankNicolsonDoDic_constructor(
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
      SEXP stdAirPressure,
      SEXP ratioDicCfix,
      SEXP ratioDicCresp,
      SEXP initialDIC,
      SEXP pCO2air,
      SEXP alkalinity
   )
   {
      int length = length(time);

      MetabCrankNicolsonDoDic* modelPointer = new MetabCrankNicolsonDoDic(
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
         length,
         asReal(ratioDicCfix),
         asReal(ratioDicCresp),
         asReal(initialDIC),
         REAL(pCO2air),
         REAL(alkalinity)
      );
      SEXP modelExtPointer = PROTECT(
         R_MakeExternalPtr(modelPointer, R_NilValue, R_NilValue)
      );

      R_RegisterCFinalizer(
         modelExtPointer,
         finalizerExternalPointer<MetabCrankNicolsonDoDic>
      );

      MetabDoDic* basePointer = dynamic_cast <MetabDoDic*> (modelPointer);
      SEXP baseExtPointer = PROTECT(
         R_MakeExternalPtr(basePointer, R_NilValue, R_NilValue)
      );

      MetabDo* basePointerDo = dynamic_cast <MetabDo*> (modelPointer);
      SEXP baseExtPointerDo = PROTECT(
         R_MakeExternalPtr(basePointerDo, R_NilValue, R_NilValue)
      );

      SEXP vecOutput = PROTECT(allocVector(VECSXP, 3));
      SET_VECTOR_ELT(vecOutput, 0, modelExtPointer);
      SET_VECTOR_ELT(vecOutput, 1, baseExtPointer);
      SET_VECTOR_ELT(vecOutput, 2, baseExtPointerDo);

      SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 3));
      SET_VECTOR_ELT(vecOutput_names, 0, install("modelExternalPointer"));
      SET_VECTOR_ELT(vecOutput_names, 1, install("baseExternalPointer"));
      SET_VECTOR_ELT(vecOutput_names, 2, install("baseExternalPointerDo"));

      setAttrib(vecOutput, install("names"), vecOutput_names);

      UNPROTECT(5);
      return vecOutput;

   }

   SEXP MetabCrankNicolsonDoDic_destructor(SEXP externalPointer)
   {

      finalizerExternalPointer<MetabCrankNicolsonDoDic>(externalPointer);

      return R_NilValue;

   }

}
