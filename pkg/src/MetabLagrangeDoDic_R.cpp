#include "metabc_R.h"

SEXP MetabLagrangeDoDic_initialize(
   SEXP baseExtPointer,
   SEXP dailyGPP,
   SEXP ratioDoCFix,
   SEXP dailyER,
   SEXP ratioDoCResp,
   SEXP k600,
   SEXP upstreamDO,
   SEXP upstreamTime,
   SEXP downstreamTime,
   SEXP upstreamTemp,
   SEXP downstreamTemp,
   SEXP upstreamPAR,
   SEXP downstreamPAR,
   SEXP parTotal,
   SEXP airPressure,
   SEXP stdAirPressure,
   SEXP timesteps,
   SEXP ratioDicCFix,
   SEXP ratioDicCResp,
   SEXP upstreamDIC,
   SEXP pCO2air,
   SEXP upstreamAlkalinity,
   SEXP downstreamAlkalinity
)
{
   int length = length(upstreamTime);

   MetabLagrangeDoDic* basePointer =
      (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExtPointer);
   basePointer->initialize(
      asReal(dailyGPP),
      asReal(ratioDoCFix),
      asReal(dailyER),
      asReal(ratioDoCResp),
      asReal(k600),
      REAL(upstreamDO),
      REAL(upstreamTime),
      REAL(downstreamTime),
      REAL(upstreamTemp),
      REAL(downstreamTemp),
      REAL(upstreamPAR),
      REAL(downstreamPAR),
      asReal(parTotal),
      REAL(airPressure),
      asReal(stdAirPressure),
      length,
      asInteger(timesteps),
      asReal(ratioDicCFix),
      asReal(ratioDicCResp),
      REAL(upstreamDIC),
      REAL(pCO2air),
      REAL(upstreamAlkalinity),
      REAL(downstreamAlkalinity)
   );

   return R_NilValue;
}

SEXP MetabLagrangeDoDic_setRatioDoCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDoDic* model = (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCFix;
   model->ratioDoCFix = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDoDic_setRatioDoCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDoDic* model = (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCResp;
   model->ratioDoCResp = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDoDic_setRatioDicCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDoDic* model = (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDicCFix;
   model->ratioDicCFix = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDoDic_setRatioDicCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDoDic* model = (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDicCResp;
   model->ratioDicCResp = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDoDic_getSummary(SEXP baseExternalPointer)
{
   MetabLagrangeDoDic* basePointer =
      (MetabLagrangeDoDic*)R_ExternalPtrAddr(baseExternalPointer);

   return MetabLagrangeDoDic_getSummary(basePointer);
}

SEXP MetabLagrangeDoDic_getSummary(MetabLagrangeDoDic* basePointer)
{
   MetabLagrangeDo* basePointerDo = dynamic_cast <MetabLagrangeDo*> (basePointer);
   SEXP oldVec = PROTECT(MetabLagrangeDo_getSummary(basePointerDo));

   SEXP vecOutput = PROTECT(VECTOR_ELT(oldVec, 0));
   SEXP vecOutputDO = PROTECT(VECTOR_ELT(oldVec, 1));

   SEXP pCO2 = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP dic = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP dicProduction = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP dicConsumption = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP co2Equilibration = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP kCO2 = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP pH = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   SEXP kH = PROTECT(allocVector(REALSXP, basePointer->numParcels));
   for(int i = 0; i < basePointer->numParcels; i++) {
      REAL(pCO2)[i] = basePointer->outputDic.pCO2[i];
      REAL(dic)[i] = basePointer->outputDic.dic[i];
      REAL(dicProduction)[i] = basePointer->outputDic.dicProduction[i];
      REAL(dicConsumption)[i] = basePointer->outputDic.dicConsumption[i];
      REAL(co2Equilibration)[i] = basePointer->outputDic.co2Equilibration[i];
      REAL(kCO2)[i] = basePointer->downstreamkCO2[i];
      REAL(pH)[i] = basePointer->outputDic.pH[i];
      REAL(kH)[i] = basePointer->downstreamkH[i];
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
   SET_VECTOR_ELT(vecOutputDIC_names, 5, install("downstreamkCO2"));
   SET_VECTOR_ELT(vecOutputDIC_names, 6, install("downstreampH"));
   SET_VECTOR_ELT(vecOutputDIC_names, 7, install("downstreamkH"));

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
