#include "metabc_R.h"

extern "C" {

   SEXP MetabForwardEulerDoDic_constructor(
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
      SEXP ratioDicCfix,
      SEXP ratioDicCresp,
      SEXP initialDIC,
      SEXP pCO2air,
      SEXP alkalinity
   )
   {
      int length = length(time);

      MetabForwardEulerDoDic* modelPointer = new MetabForwardEulerDoDic(
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
         finalizerExternalPointer<MetabForwardEulerDoDic>
      );

      UNPROTECT(1);
      return modelExtPointer;

   }

   SEXP MetabForwardEulerDoDic_destructor(SEXP externalPointer)
   {

      finalizerExternalPointer<MetabForwardEulerDoDic>(externalPointer);

      return R_NilValue;

   }

   SEXP MetabForwardEulerDoDic_run(SEXP externalPointer)
   {
      MetabForwardEulerDoDic* model = (MetabForwardEulerDoDic*)R_ExternalPtrAddr(externalPointer);
      model->run();

      SEXP oldVec = PROTECT(MetabForwardEulerDo_run_extract(model));

      SEXP vecOutput = PROTECT(VECTOR_ELT(oldVec, 0));
      SEXP vecOutputDO = PROTECT(VECTOR_ELT(oldVec, 1));

      SEXP pCO2 = PROTECT(allocVector(REALSXP, model->length));
      SEXP dic = PROTECT(allocVector(REALSXP, model->length));
      SEXP dicProduction = PROTECT(allocVector(REALSXP, model->length));
      SEXP dicConsumption = PROTECT(allocVector(REALSXP, model->length));
      SEXP co2Equilibration = PROTECT(allocVector(REALSXP, model->length));
      SEXP kCO2 = PROTECT(allocVector(REALSXP, model->length));
      SEXP pH = PROTECT(allocVector(REALSXP, model->length));
      SEXP kH = PROTECT(allocVector(REALSXP, model->length));
      for(int i = 0; i < model->length; i++) {
         REAL(pCO2)[i] = model->outputDic.pCO2[i];
         REAL(dic)[i] = model->outputDic.dic[i];
         REAL(dicProduction)[i] = model->outputDic.dicProduction[i];
         REAL(dicConsumption)[i] = model->outputDic.dicConsumption[i];
         REAL(co2Equilibration)[i] = model->outputDic.co2Equilibration[i];
         REAL(kCO2)[i] = model->kCO2[i];
         REAL(pH)[i] = model->outputDic.pH[i];
         REAL(kH)[i] = model->kH[i];
      }

      SEXP vecOutputDIC = PROTECT(allocVector(VECSXP, 8));
      SET_VECTOR_ELT(vecOutputDIC, 0, pCO2);
      SET_VECTOR_ELT(vecOutputDIC, 1, dic);
      SET_VECTOR_ELT(vecOutputDIC, 2, dicProduction);
      SET_VECTOR_ELT(vecOutputDIC, 3, dicConsumption);
      SET_VECTOR_ELT(vecOutputDIC, 4, co2Equilibration);
      SET_VECTOR_ELT(vecOutputDIC, 5, kCO2);
      SET_VECTOR_ELT(vecOutputDIC, 6, pH);
      SET_VECTOR_ELT(vecOutputDIC, 7, kH);

      SEXP vec = PROTECT(allocVector(VECSXP, 3));
      SET_VECTOR_ELT(vec, 0, vecOutput);
      SET_VECTOR_ELT(vec, 1, vecOutputDO);
      SET_VECTOR_ELT(vec, 2, vecOutputDIC);

      UNPROTECT(13);
      return vec;
   }

   SEXP MetabForwardEulerDoDic_getDt(SEXP externalPointer)
   {

      MetabForwardEulerDoDic* model = (MetabForwardEulerDoDic*)R_ExternalPtrAddr(externalPointer);
      SEXP out = PROTECT(allocVector(REALSXP, model->length));
      for (int i = 0; i < model->length; i++) {
         REAL(out)[i] = model->dt[i];
      }

      UNPROTECT(1);
      return out;

   }

}
