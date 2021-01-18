#include "metabc_R.h"

SEXP MetabDoDic_initialize(
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
   SEXP ratioDicCFix,
   SEXP ratioDicCResp,
   SEXP initialDIC,
   SEXP pCO2air,
   SEXP alkalinity
)
{
   int length = length(time);

   MetabDoDic* basePointer = (MetabDoDic*)R_ExternalPtrAddr(baseExtPointer);
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
      asReal(ratioDicCFix),
      asReal(ratioDicCResp),
      asReal(initialDIC),
      REAL(pCO2air),
      REAL(alkalinity)
   );

   return R_NilValue;
}

SEXP MetabDoDic_setRatioDoCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabDoDic* model = (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCFix_;
   model->ratioDoCFix_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDoDic_setRatioDoCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabDoDic* model = (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCResp_;
   model->ratioDoCResp_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDoDic_setRatioDicCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabDoDic* model = (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDicCFix_;
   model->ratioDicCFix_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDoDic_setRatioDicCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabDoDic* model = (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDicCResp_;
   model->ratioDicCResp_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabDoDic_getSummary(SEXP baseExternalPointer)
{
   MetabDoDic* basePointer =
      (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);

   return MetabDoDic_getSummary(basePointer);
}

SEXP MetabDoDic_getSummary(MetabDoDic* basePointer)
{
   MetabDo* basePointerDo = dynamic_cast <MetabDo*> (basePointer);
   SEXP oldVec = PROTECT(MetabDo_getSummary(basePointerDo));

   SEXP vecOutput = PROTECT(VECTOR_ELT(oldVec, 0));
   SEXP vecOutputDO = PROTECT(VECTOR_ELT(oldVec, 1));

   SEXP pCO2 = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP dic = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP dicProduction = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP dicConsumption = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP co2Equilibration = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP kCO2 = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP pH = PROTECT(allocVector(REALSXP, basePointer->length_));
   SEXP kH = PROTECT(allocVector(REALSXP, basePointer->length_));
   for(int i = 0; i < basePointer->length_; i++) {
      REAL(pCO2)[i] = basePointer->outputDic_.pCO2[i];
      REAL(dic)[i] = basePointer->outputDic_.dic[i];
      REAL(dicProduction)[i] = basePointer->outputDic_.dicProduction[i];
      REAL(dicConsumption)[i] = basePointer->outputDic_.dicConsumption[i];
      REAL(co2Equilibration)[i] = basePointer->outputDic_.co2Equilibration[i];
      REAL(kCO2)[i] = basePointer->kCO2_[i];
      REAL(pH)[i] = basePointer->outputDic_.pH[i];
      REAL(kH)[i] = basePointer->kH_[i];
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

   SEXP vecOutputDIC_names = PROTECT(allocVector(VECSXP, 8));
   SET_VECTOR_ELT(vecOutputDIC_names, 0, install("pCO2"));
   SET_VECTOR_ELT(vecOutputDIC_names, 1, install("dic"));
   SET_VECTOR_ELT(vecOutputDIC_names, 2, install("dicProduction"));
   SET_VECTOR_ELT(vecOutputDIC_names, 3, install("dicConsumption"));
   SET_VECTOR_ELT(vecOutputDIC_names, 4, install("co2Equilibration"));
   SET_VECTOR_ELT(vecOutputDIC_names, 5, install("kCO2"));
   SET_VECTOR_ELT(vecOutputDIC_names, 6, install("pH"));
   SET_VECTOR_ELT(vecOutputDIC_names, 7, install("kH"));

   setAttrib(vecOutputDIC, install("names"), vecOutputDIC_names);

   SEXP vec = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vec, 0, vecOutput);
   SET_VECTOR_ELT(vec, 1, vecOutputDO);
   SET_VECTOR_ELT(vec, 2, vecOutputDIC);

   SEXP vec_names = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vec_names, 0, install("output"));
   SET_VECTOR_ELT(vec_names, 1, install("outputDo"));
   SET_VECTOR_ELT(vec_names, 2, install("outputDic"));

   setAttrib(vec, install("names"), vec_names);

   UNPROTECT(15);
   return vec;
}
