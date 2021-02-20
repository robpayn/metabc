library(metabc)

# Test raw interface ####

cobject <- .Call(
   "CarbonateEq_constructor", 
   10, 
   0
)

test <- .Call(
   "CarbonateEq_optfCO2FromDICTotalAlk",
   cobject,
   2400 * 1e-6,
   2500 * 1e-6,
   1e-5,
   2,
   12
)

.Call(
   "CarbonateEq_reset",
   cobject,
   15,
   0
)

test2 <- .Call(
   "CarbonateEq_optfCO2FromDICTotalAlk",
   cobject,
   2400 * 1e-6,
   2500 * 1e-6,
   1e-5,
   2,
   12
)

# Test R6 wrapper ####

r6object <- CCarbonateEq$new(tempC = 10)
test3 <- r6object$optfCO2FromDICTotalAlk(
   2400 * 1e-6,
   2500 * 1e-6,
)

r6object$reset(15)
test4 <- r6object$optfCO2FromDICTotalAlk(
   2400 * 1e-6,
   2500 * 1e-6,
)
