#include <R.h>
#include <Rinternals.h>
#include "metabc.h"

template <typename T>
void finalizerExternalPointer(SEXP externalPointer) {

   T* pointer = (T*)R_ExternalPtrAddr(externalPointer);
   R_ClearExternalPtr(externalPointer);
   delete pointer;

}

SEXP MetabDo_getSummary(MetabDo*);

SEXP MetabDoDic_getSummary(MetabDoDic*);

extern "C"
{
   SEXP MetabDo_run(SEXP);

   SEXP MetabDo_getSummary(SEXP);

   SEXP MetabDo_getPARDist(SEXP externalPointer);

   SEXP MetabDo_getDt(SEXP externalPointer);

   SEXP MetabDo_setDailyGPP(SEXP externalPointer, SEXP value);

   SEXP MetabForwardEulerDo_constructor(
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
         SEXP stdAirPressure
   );

   SEXP MetabForwardEulerDo_destructor(SEXP externalPointer);

   SEXP MetabCrankNicolsonDo_constructor(
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
         SEXP stdAirPressure
   );

   SEXP MetabCrankNicolsonDo_destructor(SEXP);

   SEXP MetabDoDic_run(SEXP);

   SEXP MetabDoDic_getSummary(SEXP);

   SEXP MetabForwardEulerDoDic_constructor(
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
   );

   SEXP MetabForwardEulerDoDic_destructor(SEXP externalPointer);

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
   );

   SEXP MetabCrankNicolsonDoDic_destructor(SEXP externalPointer);
}
