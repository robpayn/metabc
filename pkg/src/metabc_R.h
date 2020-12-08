#include <R.h>
#include <Rinternals.h>
#include "metabc.h"

template <typename T>
void finalizerExternalPointer(SEXP externalPointer) {

   T* pointer = (T*)R_ExternalPtrAddr(externalPointer);
   R_ClearExternalPtr(externalPointer);
   delete pointer;

}

extern "C"
{
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
   );

   SEXP MetabForwardEulerDo_destructor(SEXP externalPointer);

   SEXP MetabForwardEulerDo_run(SEXP externalPointer);

   SEXP MetabForwardEulerDo_run_extract(MetabForwardEulerDo* model);

   SEXP MetabForwardEulerDo_getPAR(SEXP externalPointer);

   SEXP MetabForwardEulerDo_getDt(SEXP externalPointer);

   SEXP MetabForwardEulerDo_setDailyGPP(SEXP externalPointer, SEXP value);

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
   );

   SEXP MetabForwardEulerDoDic_destructor(SEXP externalPointer);

   SEXP MetabForwardEulerDoDic_run(SEXP externalPointer);

   SEXP MetabForwardEulerDoDic_getDt(SEXP externalPointer);
}
