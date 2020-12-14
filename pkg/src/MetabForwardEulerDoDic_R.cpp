#include "metabc_R.h"

SEXP MetabForwardEulerDoDic_constructor()
{
   return Metab_constructor<MetabForwardEulerDoDic, MetabDoDic>();
}

SEXP MetabForwardEulerDoDic_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabForwardEulerDoDic>(externalPointer);

   return R_NilValue;
}
