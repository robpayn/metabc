rm(list = ls())

library(metab)
library(metabc)

load(file = "./2014-09-13/signal.RData")

timeStart <- Sys.time()

cppModel <- CppOneStationMetabDoDic$new(
   dailyGPP = 610,
   dailyER = 275,
   k600 = 19.5,
   airPressure = 638,
   initialDO = signalOut$getVariable("do")[1],
   time = signalOut$time,
   temp = signalOut$getVariable("temp"),
   par = signalOut$getVariable("par"),
   initialDIC = signalOut$getVariable("dic")[1],
   pCO2air = 400,
   alkalinity = 2857,
)

cppModel$run()

timers <- list(cppTime = Sys.time() - timeStart)

timeStart <- Sys.time()

rModel <- OneStationMetabDoDic$new(
   dailyGPP = 610,
   dailyER = 275,
   k600 = 19.5,
   airPressure = 638,
   initialDO = signalOut$getVariable("do")[1],
   time = signalOut$time,
   temp = signalOut$getVariable("temp"),
   par = signalOut$getVariable("par"),
   initialDIC = signalOut$getVariable("dic")[1],
   pCO2air = 400,
   alkalinity = 2857,
)

rModel$run()

timers$rTime <- Sys.time() - timeStart

windows(width = 8, height = 11)
par(mfrow = c(2, 1), mar = c(4.5, 4.5, 1, 1))

plot(
   cppModel$output$time,
   cppModel$output$pCO2,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$pCO2,
   col = "red"
)

plot(
   cppModel$output$time,
   cppModel$output$dic,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$dic,
   col = "red"
)
