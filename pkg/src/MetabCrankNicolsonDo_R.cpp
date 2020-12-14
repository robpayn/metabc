#include "metabc_R.h"

SEXP MetabCrankNicolsonDo_constructor()
{
   return Metab_constructor<MetabCrankNicolsonDo, MetabDo>();
}

SEXP MetabCrankNicolsonDo_destructor(SEXP modelExtPointer)
{
   finalizerExternalPointer<MetabCrankNicolsonDo>(modelExtPointer);

   return R_NilValue;
}
