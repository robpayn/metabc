#include "metabc_R.h"

SEXP MetabDoDic_run(SEXP baseExternalPointer)
{
   MetabDoDic* basePointer =
      (MetabDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   basePointer->run();

   return MetabDoDic_getSummary(basePointer);
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

   SEXP pCO2 = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP dic = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP dicProduction = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP dicConsumption = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP co2Equilibration = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP kCO2 = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP pH = PROTECT(allocVector(REALSXP, basePointer->length));
   SEXP kH = PROTECT(allocVector(REALSXP, basePointer->length));
   for(int i = 0; i < basePointer->length; i++) {
      REAL(pCO2)[i] = basePointer->outputDic.pCO2[i];
      REAL(dic)[i] = basePointer->outputDic.dic[i];
      REAL(dicProduction)[i] = basePointer->outputDic.dicProduction[i];
      REAL(dicConsumption)[i] = basePointer->outputDic.dicConsumption[i];
      REAL(co2Equilibration)[i] = basePointer->outputDic.co2Equilibration[i];
      REAL(kCO2)[i] = basePointer->kCO2[i];
      REAL(pH)[i] = basePointer->outputDic.pH[i];
      REAL(kH)[i] = basePointer->kH[i];
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
