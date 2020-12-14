#include "metabc_R.h"

SEXP MetabLagrangeCNOneStepDo_constructor()
{
   return Metab_constructor<MetabLagrangeCNOneStepDo, MetabLagrangeDo>();
}

SEXP MetabLagrangeCNOneStepDo_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabLagrangeCNOneStepDo>(externalPointer);

   return R_NilValue;
}
