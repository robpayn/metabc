rm(list = ls())

library(metab)
library(metabc)

load(file = "./2014-09-13/signal.RData")

# Lagrange model ####

timeStart <- Sys.time()

cppModel <- CppOneStationMetabDo$new(
   dailyGPP = 610,
   dailyER = 275,
   k600 = 19.5,
   airPressure = 638,
   initialDO = signalOut$getVariable("do")[1],
   time = signalOut$time,
   temp = signalOut$getVariable("temp"),
   par = signalOut$getVariable("par")
)

cppModel$run()

timers <- list(cppTime = Sys.time() - timeStart)

timeStart <- Sys.time()

rModel <- OneStationMetabDo$new(
   dailyGPP = 610,
   dailyER = 275,
   k600 = 19.5,
   airPressure = 638,
   initialDO = signalOut$getVariable("do")[1],
   time = signalOut$time,
   temp = signalOut$getVariable("temp"),
   par = signalOut$getVariable("par")
)

rModel$run()

timers$rTime <- Sys.time() - timeStart

windows(width = 8, height = 6)

plot(
   cppModel$output$time,
   cppModel$output$do,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$do,
   col = "red"
)
