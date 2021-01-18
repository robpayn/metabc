#include "metabc_R.h"

SEXP MetabLagrangeDo_initialize(
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
   SEXP gwDOEnable,
   SEXP gwAlpha,
   SEXP gwDO
)
{
   int length = length(upstreamTime);

   MetabLagrangeDo* basePointer =
      (MetabLagrangeDo*)R_ExternalPtrAddr(baseExtPointer);

   if (asLogical(gwDOEnable)) {
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
         asInteger(timesteps)
      );
   }

   return R_NilValue;
}

SEXP MetabLagrangeDo_setRatioDoCFix(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDo* model = (MetabLagrangeDo*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCFix_;
   model->ratioDoCFix_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDo_setRatioDoCResp(SEXP baseExternalPointer, SEXP value)
{
   MetabLagrangeDo* model = (MetabLagrangeDo*)R_ExternalPtrAddr(baseExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->ratioDoCResp_;
   model->ratioDoCResp_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP MetabLagrangeDo_getSummary(SEXP baseExtPointer)
{
   MetabLagrangeDo* basePointer = (MetabLagrangeDo*)R_ExternalPtrAddr(baseExtPointer);
   return MetabLagrangeDo_getSummary(basePointer);
}

SEXP MetabLagrangeDo_getSummary(MetabLagrangeDo* basePointer)
{
   SEXP travelTimes = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP cFixation = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP cRespiration = PROTECT(allocVector(REALSXP, basePointer->numParcels_));

   SEXP dox = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP upstreamDoSat = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP downstreamDoSat = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP doProduction = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP doConsumption = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP upstreamkDo = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP downstreamkDo = PROTECT(allocVector(REALSXP, basePointer->numParcels_));
   SEXP doEquilibration = PROTECT(allocVector(REALSXP, basePointer->numParcels_));

   for(int i = 0; i < basePointer->numParcels_; i++) {
      REAL(travelTimes)[i] = basePointer->travelTimes_[i];
      REAL(cFixation)[i] = basePointer->output_.cFixation[i];
      REAL(cRespiration)[i] = basePointer->output_.cRespiration[i];

      REAL(dox)[i] = basePointer->outputDo_.dox[i];
      REAL(upstreamDoSat)[i] = basePointer->upstreamSatDo_[i];
      REAL(downstreamDoSat)[i] = basePointer->downstreamSatDo_[i];
      REAL(doProduction)[i] = basePointer->outputDo_.doProduction[i];
      REAL(doConsumption)[i] = basePointer->outputDo_.doConsumption[i];
      REAL(upstreamkDo)[i] = basePointer->upstreamkDo_[i];
      REAL(downstreamkDo)[i] = basePointer->upstreamkDo_[i];
      REAL(doEquilibration)[i] = basePointer->outputDo_.doEquilibration[i];
   }

   SEXP vecOutput = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput, 0, travelTimes);
   SET_VECTOR_ELT(vecOutput, 1, cFixation);
   SET_VECTOR_ELT(vecOutput, 2, cRespiration);

   SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput_names, 0, install("travelTimes"));
   SET_VECTOR_ELT(vecOutput_names, 1, install("cFixation"));
   SET_VECTOR_ELT(vecOutput_names, 2, install("cRespiration"));

   setAttrib(vecOutput, install("names"), vecOutput_names);

   SEXP vecOutputDO = PROTECT(allocVector(VECSXP, 8));
   SET_VECTOR_ELT(vecOutputDO, 0, dox);
   SET_VECTOR_ELT(vecOutputDO, 1, upstreamDoSat);
   SET_VECTOR_ELT(vecOutputDO, 2, downstreamDoSat);
   SET_VECTOR_ELT(vecOutputDO, 3, doProduction);
   SET_VECTOR_ELT(vecOutputDO, 4, doConsumption);
   SET_VECTOR_ELT(vecOutputDO, 5, upstreamkDo);
   SET_VECTOR_ELT(vecOutputDO, 6, downstreamkDo);
   SET_VECTOR_ELT(vecOutputDO, 7, doEquilibration);

   SEXP vecOutputDO_names = PROTECT(allocVector(VECSXP, 8));
   SET_VECTOR_ELT(vecOutputDO_names, 0, install("dox"));
   SET_VECTOR_ELT(vecOutputDO_names, 1, install("upstreamDoSat"));
   SET_VECTOR_ELT(vecOutputDO_names, 2, install("downstreamDoSat"));
   SET_VECTOR_ELT(vecOutputDO_names, 3, install("doProduction"));
   SET_VECTOR_ELT(vecOutputDO_names, 4, install("doConsumption"));
   SET_VECTOR_ELT(vecOutputDO_names, 5, install("upstreamkDo"));
   SET_VECTOR_ELT(vecOutputDO_names, 6, install("downstreamkDo"));
   SET_VECTOR_ELT(vecOutputDO_names, 7, install("doEquilibration"));

   setAttrib(vecOutputDO, install("names"), vecOutputDO_names);

   SEXP vec = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vec, 0, vecOutput);
   SET_VECTOR_ELT(vec, 1, vecOutputDO);

   SEXP vec_names = PROTECT(allocVector(VECSXP, 2));
   SET_VECTOR_ELT(vec_names, 0, install("output"));
   SET_VECTOR_ELT(vec_names, 1, install("outputDo"));

   setAttrib(vec, install("names"), vec_names);

   UNPROTECT(17);
   return vec;
}

