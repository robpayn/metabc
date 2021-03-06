#include "metabc_R.h"

SEXP Metab_run(SEXP metabExtPointer)
{
   Metab* metabPointer = (Metab*)R_ExternalPtrAddr(metabExtPointer);
   metabPointer->run();

   return R_NilValue;
}

SEXP Metab_setDailyGPP(SEXP metabExternalPointer, SEXP value)
{
   Metab* model = (Metab*)R_ExternalPtrAddr(metabExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->dailyGPP_;
   model->dailyGPP_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP Metab_setDailyER(SEXP metabExternalPointer, SEXP value)
{
   Metab* model = (Metab*)R_ExternalPtrAddr(metabExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model-> dailyER_;
   model->dailyER_ = asReal(value);

   UNPROTECT(1);
   return out;
}

SEXP Metab_setk600(SEXP metabExternalPointer, SEXP value)
{
   Metab* model = (Metab*)R_ExternalPtrAddr(metabExternalPointer);
   SEXP out = PROTECT(allocVector(REALSXP, 1));
   REAL(out)[0] = model->k600_;
   model->k600_ = asReal(value);

   UNPROTECT(1);
   return out;
}
