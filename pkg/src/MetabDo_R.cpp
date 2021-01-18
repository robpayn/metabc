#include "metabc_R.h"

SEXP MetabDo_initialize
(
   SEXP baseExtPointer,
   SEXP dailyGPP,
   SEXP ratioDoCFix,
   SEXP dailyER,
   SEXP ratioDoCResp,
   SEXP k600,
   SEXP initialDO,
   SEXP time,
   SEXP temp,
   SEXP par,
   SEXP parTotal,
   SEXP airPressure,
   SEXP stdAirPressure,
   SEXP gwDOEnable,
   SEXP gwAlpha,
   SEXP gwDO
)
{
   int length = length(time);

   MetabDo* basePointer = (MetabDo*)R_ExternalPtrAddr(baseExtPointer);

   if (asLogical(gwDOEnable)) {
      basePointer->initialize(
         asReal(dailyGPP),
         asReal(ratioDoCFix),
         asReal(dailyER),
         asReal(ratioDoCResp),
         asReal(k600),
         asReal(initialDO),
         REAL(time),
         REAL(temp),
         REAL(par),
         asReal(parTotal),
         REAL(airPressure),
         asReal(stdAirPressure),
         length,
         REAL(gwAlpha),
         REAL(gwDO)
      );
   } else {
      basePointer->initialize(
         asReal(dailyGPP),
         asReal(ratioDoCFix),
         asReal(dailyER),
         asReal(ratioDoCResp),
         asReal(k600),
         asReal(initialDO),
         REAL(time),
         REAL(temp),
         REAL(par),
         asReal(parTotal),
         REAL(airPressure),
         asReal(stdAirPressure),
         length
      );
   }

   return R_NilValue;
}

SEXP MetabDo_setRatioDoCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabDo* model = (MetabDo*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCFix_;
   model->ratioDoCFix_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDo_setRatioDoCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabDo* model = (MetabDo*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCResp_;
   model->ratioDoCResp_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDo_getSummary(SEXP baseExtPointer)
{
   MetabDo* basePointer = (MetabDo*)R_ExternalPtrAddr(baseExtPointer);
   return MetabDo_getSummary(basePointer);
}

SEXP MetabDo_getSummary(MetabDo* basePointer)
{
   SEXP dt = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP cFixation = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP cRespiration = PROTECT(allocVector(REALSXP, basePointer->length_));

   SEXP dox = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP doSat = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP doProduction = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP doConsumption = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP k = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP doEquilibration = PROTECT(allocVector(REALSXP, basePointer->length_));

   for(int i = 0; i < basePointer->length_; i++) {
      REAL(dt)[i] = basePointer->dt_[i];
      REAL(cFixation)[i] = basePointer->output_.cFixation[i];
      REAL(cRespiration)[i] = basePointer->output_.cRespiration[i];

      REAL(dox)[i] = basePointer->outputDo_.dox[i];
      REAL(doSat)[i] = basePointer->satDo_[i];
      REAL(doProduction)[i] = basePointer->outputDo_.doProduction[i];
      REAL(doConsumption)[i] = basePointer->outputDo_.doConsumption[i];
      REAL(k)[i] = basePointer->kDo_[i];
      REAL(doEquilibration)[i] = basePointer->outputDo_.doEquilibration[i];
   }

   SEXP vecOutput = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput, 0, dt);
   SET_VECTOR_ELT(vecOutput, 1, cFixation);
   SET_VECTOR_ELT(vecOutput, 2, cRespiration);

   SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput_names, 0, install("dt"));
   SET_VECTOR_ELT(vecOutput_names, 1, install("cFixation"));
   SET_VECTOR_ELT(vecOutput_names, 2, install("cRespiration"));

   setAttrib(vecOutput, install("names"), vecOutput_names);

   SEXP vecOutputDO = PROTECT(allocVector(VECSXP, 6));
   SET_VECTOR_ELT(vecOutputDO, 0, dox);
   SET_VECTOR_ELT(vecOutputDO, 1, doSat);
   SET_VECTOR_ELT(vecOutputDO, 2, doProduction);
   SET_VECTOR_ELT(vecOutputDO, 3, doConsumption);
   SET_VECTOR_ELT(vecOutputDO, 4, k);
   SET_VECTOR_ELT(vecOutputDO, 5, doEquilibration);

   SEXP vecOutputDO_names = PROTECT(allocVector(VECSXP, 6));
   SET_VECTOR_ELT(vecOutputDO_names, 0, install("dox"));
   SET_VECTOR_ELT(vecOutputDO_names, 1, install("doSat"));
   SET_VECTOR_ELT(vecOutputDO_names, 2, install("doProduction"));
   SET_VECTOR_ELT(vecOutputDO_names, 3, install("doConsumption"));
   SET_VECTOR_ELT(vecOutputDO_names, 4, install("k"));
   SET_VECTOR_ELT(vecOutputDO_names, 5, install("doEquilibration"));

   setAttrib(vecOutputDO, install("names"), vecOutputDO_names);

   SEXP vec = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vec, 0, vecOutput);
   SET_VECTOR_ELT(vec, 1, vecOutputDO);

   SEXP vec_names = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vec_names, 0, install("output"));
   SET_VECTOR_ELT(vec_names, 1, install("outputDo"));

   setAttrib(vec, install("names"), vec_names);

   UNPROTECT(15);
   return vec;
}

SEXP MetabDo_getPARDist(SEXP externalPointer)
{
   MetabDo* model = (MetabDo*)R_ExternalPtrAddr(externalPointer);

   SEXP out = PROTECT(allocVector(REALSXP, model->length_));
   for (int i = 0; i < model->length_; i++) {
      REAL(out)[i] = model->parDist_[i];
   }

   UNPROTECT(1);
   return out;
}

SEXP MetabDo_getDt(SEXP externalPointer)
{
   MetabDo* model = (MetabDo*)R_ExternalPtrAddr(externalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, model->length_));
   for (int i = 0; i < model->length_; i++) {
      REAL(out)[i] = model->dt_[i];
   }

   UNPROTECT(1);
   return out;
}
