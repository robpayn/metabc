#include "metabc_R.h"

SEXP MetabLagrangeGenericDo_constructor_CN()
{
   return Metab_constructor<MetabLagrangeGenericDo<MetabCrankNicolsonDo>, MetabLagrangeDo>();
}

SEXP MetabLagrangeGenericDo_destructor_CN(SEXP externalPointer)
{
   finalizerExternalPointer<MetabLagrangeGenericDo<MetabCrankNicolsonDo>>(externalPointer);

   return R_NilValue;
}
