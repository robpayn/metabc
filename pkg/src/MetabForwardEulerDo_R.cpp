#include "metabc_R.h"

SEXP MetabForwardEulerDo_constructor()
{
   return Metab_constructor<MetabForwardEulerDo, MetabDo>();
}

SEXP MetabForwardEulerDo_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabForwardEulerDo>(externalPointer);

   return R_NilValue;
}
