#include <R.h>
#include <Rinternals.h>
#include "metabc_R.h"

SEXP MetabForwardEulerDo_constructor(
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
   SEXP stdAirPressure
)
{
   int length = length(time);

   MetabForwardEulerDo* modelPointer = new MetabForwardEulerDo(
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
   SEXP modelExtPointer = PROTECT(
      R_MakeExternalPtr(modelPointer, R_NilValue, R_NilValue)
   );

   R_RegisterCFinalizer(
      modelExtPointer,
      finalizerExternalPointer<MetabForwardEulerDo>
   );

   UNPROTECT(1);
   return modelExtPointer;
}

SEXP MetabForwardEulerDo_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabForwardEulerDo>(externalPointer);

   return R_NilValue;
}

SEXP MetabForwardEulerDo_run(SEXP externalPointer)
{
   MetabForwardEulerDo* model = (MetabForwardEulerDo*)R_ExternalPtrAddr(externalPointer);
   model->run();

   return MetabForwardEulerDo_run_extract(model);
}

SEXP MetabForwardEulerDo_run_extract(MetabForwardEulerDo* model)
{
   SEXP dox = PROTECT(allocVector(REALSXP, model->length));
   SEXP doSat = PROTECT(allocVector(REALSXP, model->length));
   SEXP cFixation = PROTECT(allocVector(REALSXP, model->length));
   SEXP cRespiration = PROTECT(allocVector(REALSXP, model->length));
   SEXP doProduction = PROTECT(allocVector(REALSXP, model->length));
   SEXP doConsumption = PROTECT(allocVector(REALSXP, model->length));
   SEXP k = PROTECT(allocVector(REALSXP, model->length));
   SEXP doEquilibration = PROTECT(allocVector(REALSXP, model->length));

   for(int i = 0; i < model->length; i++) {
      REAL(dox)[i] = model->outputDo.dox[i];
      REAL(doSat)[i] = model->satDo[i];
      REAL(cFixation)[i] = model->output.cFixation[i];
      REAL(cRespiration)[i] = model->output.cRespiration[i];
      REAL(doProduction)[i] = model->outputDo.doProduction[i];
      REAL(doConsumption)[i] = model->outputDo.doConsumption[i];
      REAL(k)[i] = model->kDo[i];
      REAL(doEquilibration)[i] = model->outputDo.doEquilibration[i];
   }

   SEXP vecOutput = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vecOutput, 0, cFixation);
   SET_VECTOR_ELT(vecOutput, 1, cRespiration);
   SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vecOutput_names, 0, install("cFixation"));
   SET_VECTOR_ELT(vecOutput_names, 1, install("cRespiration"));
   setAttrib(vecOutput, install("names"), vecOutput_names);

   SEXP vecOutputDO = PROTECT(allocVector(VECSXP, 6));
   SET_VECTOR_ELT(vecOutputDO, 0, dox);
   SET_VECTOR_ELT(vecOutputDO, 1, doSat);
   SET_VECTOR_ELT(vecOutputDO, 2, doProduction);
   SET_VECTOR_ELT(vecOutputDO, 3, doConsumption);
   SET_VECTOR_ELT(vecOutputDO, 4, k);
   SET_VECTOR_ELT(vecOutputDO, 5, doEquilibration);

   SEXP vec = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vec, 0, vecOutput);
   SET_VECTOR_ELT(vec, 1, vecOutputDO);

   UNPROTECT(12);
   return vec;
}

SEXP MetabForwardEulerDo_getPAR(SEXP externalPointer)
{
   MetabForwardEulerDo* model = (MetabForwardEulerDo*)R_ExternalPtrAddr(externalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, model->length));
   for (int i = 0; i < model->length; i++) {
      REAL(out)[i] = model->par[i];
   }

   UNPROTECT(1);
   return out;
}

SEXP MetabForwardEulerDo_getDt(SEXP externalPointer)
{
   MetabForwardEulerDo* model = (MetabForwardEulerDo*)R_ExternalPtrAddr(externalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, model->length));
   for (int i = 0; i < model->length; i++) {
      REAL(out)[i] = model->dt[i];
   }

   UNPROTECT(1);
   return out;
}

SEXP MetabForwardEulerDo_setDailyGPP(SEXP externalPointer, SEXP value)
{
   MetabForwardEulerDo* model = (MetabForwardEulerDo*)R_ExternalPtrAddr(externalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->dailyGPP;
   model->dailyGPP = asReal(value);

   UNPROTECT(1);
   return out;
}
