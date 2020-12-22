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
   T* modelPointer = new T();
   B* basePointer = dynamic_cast <B*> (modelPointer);
   Metab* metabPointer = dynamic_cast <Metab*> (basePointer);

   SEXP modelExtPointer = PROTECT(
      R_MakeExternalPtr(modelPointer, R_NilValue, R_NilValue)
   );

   R_RegisterCFinalizer(
      modelExtPointer,
      finalizerExternalPointer<T>
   );

   SEXP baseExtPointer = PROTECT(
      R_MakeExternalPtr(basePointer, R_NilValue, R_NilValue)
   );
   SEXP metabExtPointer = PROTECT(
      R_MakeExternalPtr(metabPointer, R_NilValue, R_NilValue)
   );

   SEXP vecOutput = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput, 0, modelExtPointer);
   SET_VECTOR_ELT(vecOutput, 1, baseExtPointer);
   SET_VECTOR_ELT(vecOutput, 2, metabExtPointer);

   SEXP vecOutput_names = PROTECT(allocVector(VECSXP, 3));
   SET_VECTOR_ELT(vecOutput_names, 0, install("modelExternalPointer"));
   SET_VECTOR_ELT(vecOutput_names, 1, install("baseExternalPointer"));
   SET_VECTOR_ELT(vecOutput_names, 2, install("metabExternalPointer"));

   setAttrib(vecOutput, install("names"), vecOutput_names);

   UNPROTECT(5);
   return vecOutput;
}

SEXP MetabDo_getSummary(MetabDo*);

SEXP MetabDoDic_getSummary(MetabDoDic*);

SEXP MetabLagrangeDo_getSummary(MetabLagrangeDo*);

SEXP MetabLagrangeDoDic_getSummary(MetabLagrangeDoDic*);

extern "C"
{
   SEXP Metab_run(SEXP metabExternalPointer);

   SEXP Metab_setDailyGPP(SEXP metabExternalPointer, SEXP value);

   SEXP Metab_setDailyER(SEXP metabExternalPointer, SEXP value);

   SEXP Metab_setk600(SEXP metabExternalPointer, SEXP value);

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

   SEXP MetabDo_setRatioDoCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabDo_setRatioDoCResp(SEXP baseExternalPointer, SEXP value);

   SEXP MetabDo_getSummary(SEXP);

   SEXP MetabDo_getPARDist(SEXP externalPointer);

   SEXP MetabDo_getDt(SEXP externalPointer);

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

   SEXP MetabLagrangeDo_setRatioDoCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabLagrangeDo_setRatioDoCResp(SEXP baseExternalPointer, SEXP value);

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

   SEXP MetabDoDic_setRatioDoCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabDoDic_setRatioDoCResp(SEXP baseExternalPointer, SEXP value);

   SEXP MetabDoDic_setRatioDicCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabDoDic_setRatioDicCResp(SEXP baseExternalPointer, SEXP value);

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

   SEXP MetabLagrangeDoDic_setRatioDoCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabLagrangeDoDic_setRatioDoCResp(SEXP baseExternalPointer, SEXP value);

   SEXP MetabLagrangeDoDic_setRatioDicCFix(SEXP baseExternalPointer, SEXP value);

   SEXP MetabLagrangeDoDic_setRatioDicCResp(SEXP baseExternalPointer, SEXP value);

   SEXP MetabLagrangeDoDic_getSummary(SEXP);

   SEXP MetabLagrangeCNOneStepDoDic_constructor();

   SEXP MetabLagrangeCNOneStepDoDic_destructor(SEXP externalPointer);
}
