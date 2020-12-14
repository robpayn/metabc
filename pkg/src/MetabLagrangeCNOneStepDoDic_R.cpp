#include "metabc_R.h"

SEXP MetabLagrangeCNOneStepDoDic_constructor()
{
   return Metab_constructor<MetabLagrangeCNOneStepDoDic, MetabLagrangeDoDic>();
}

SEXP MetabLagrangeCNOneStepDoDic_destructor(SEXP externalPointer)
{
   finalizerExternalPointer<MetabLagrangeCNOneStepDoDic>(externalPointer);

   return R_NilValue;
}
