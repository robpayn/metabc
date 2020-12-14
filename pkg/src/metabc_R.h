#include <R.h>
#include <Rinternals.h>
#include "metabc.h"

template <class T>
void finalizerExternalPointer(SEXP externalPointer) {

   T* pointer = (T*)R_ExternalPtrAddr(externalPointer);
   R_ClearExternalPtr(externalPointer);
   delete pointer;

}

template <class T, class B>
SEXP Metab_constructor()
{
   T* genericPointer = new T();
   B* modelPointer = dynamic_cast <B*> (genericPointer);

   SEXP modelExtPointer = PROTECT(
      R_MakeExternalPtr(genericPointer, R_NilValue, R_NilValue)
   );

   R_RegisterCFinalizer(
      modelExtPointer,
      finalizerExternalPointer<T>
   );

   SEXP baseExtPointer = PROTECT(
      R_MakeExternalPtr(modelPointer, R_NilValue, R_NilValue)
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

SEXP MetabDo_getSummary(MetabDo*);

SEXP MetabDoDic_getSummary(MetabDoDic*);

SEXP MetabLagrangeDo_getSummary(MetabLagrangeDo*);

SEXP MetabLagrangeDoDic_getSummary(MetabLagrangeDoDic*);

extern "C"
{
   SEXP MetabDo_initialize(
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
      SEXP stdAirPressure
   );

   SEXP MetabDo_run(SEXP);

   SEXP MetabDo_getSummary(SEXP);

   SEXP MetabDo_getPARDist(SEXP externalPointer);

   SEXP MetabDo_getDt(SEXP externalPointer);

   SEXP MetabDo_setDailyGPP(SEXP externalPointer, SEXP value);

   SEXP MetabForwardEulerDo_constructor();

   SEXP MetabForwardEulerDo_destructor(SEXP externalPointer);

   SEXP MetabCrankNicolsonDo_constructor();

   SEXP MetabCrankNicolsonDo_destructor(SEXP);

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
      SEXP timesteps
   );

   SEXP MetabLagrangeDo_run(SEXP);

   SEXP MetabLagrangeDo_getSummary(SEXP);

   SEXP MetabLagrangeCNOneStepDo_constructor();

   SEXP MetabLagrangeCNOneStepDo_destructor(SEXP externalPointer);

   SEXP MetabLagrangeGenericDo_constructor_CN();

   SEXP MetabLagrangeGenericDo_destructor_CN(SEXP externalPointer);

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
   );

   SEXP MetabDoDic_run(SEXP);

   SEXP MetabDoDic_getSummary(SEXP);

   SEXP MetabForwardEulerDoDic_constructor();

   SEXP MetabForwardEulerDoDic_destructor(SEXP externalPointer);

   SEXP MetabCrankNicolsonDoDic_constructor();

   SEXP MetabCrankNicolsonDoDic_destructor(SEXP externalPointer);

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
   );

   SEXP MetabLagrangeDoDic_run(SEXP);

   SEXP MetabLagrangeDoDic_getSummary(SEXP);

   SEXP MetabLagrangeCNOneStepDoDic_constructor();

   SEXP MetabLagrangeCNOneStepDoDic_destructor(SEXP externalPointer);
}
