#include "metabc_R.h"

SEXP MetabCrankNicolsonDoDic_constructor()
{
   return Metab_constructor<MetabCrankNicolsonDoDic, MetabDoDic>();
}

SEXP MetabCrankNicolsonDoDic_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabCrankNicolsonDoDic>(externalPointer);

   return R_NilValue;
}
