rm(list = ls())

library(metab)
library(metabc)
library(bench)

load(file = "./2014-09-13/signal.RData")

gpp <- 610
gppdo <- 1
er <- 275
erdo <- -1
k600 <- 19.5
length <- length(signalOut$time)
initDO <- signalOut$getVariable("do")[1];
time <- as.numeric(signalOut$time) / 86400
temp <- signalOut$getVariable("temp")
par <- signalOut$getVariable("par")
parTotal <- -1
airPressure <- rep(638, length)
stdAirPressure <- 760

timers <- list(cppTimeFE = bench_time({
   
   cppModelFE <- .Call("MetabForwardEulerDo_constructor")
   .Call(
      "MetabDo_initialize",
      cppModelFE$baseExternalPointer,
      gpp,
      gppdo,
      er,
      erdo,
      k600,
      initDO,
      time,
      temp,
      par,
      parTotal,
      airPressure,
      stdAirPressure
   )
   
   cppModelFE_output <- .Call("MetabDo_run", cppModelFE$baseExternalPointer);

}))

timers$cppTimeCN <- bench_time({

   cppModelCN <- .Call("MetabCrankNicolsonDo_constructor")
   .Call(
      "MetabDo_initialize",
      cppModelCN$baseExternalPointer,
      gpp,
      gppdo,
      er,
      erdo,
      k600,
      initDO,
      time,
      temp,
      par,
      parTotal,
      airPressure,
      stdAirPressure
   )
   
   cppModelCN_output <- .Call("MetabDo_run", cppModelCN$baseExternalPointer)

})

timers$rTime <- bench_time({
   
   rModel <- OneStationMetabDo$new(
      dailyGPP = gpp,
      ratioDoCfix = gppdo,
      dailyER = er,
      ratioDoCresp = erdo,
      k600 = k600,
      airPressure = airPressure[1],
      stdAirPressure = stdAirPressure,
      initialDO = initDO,
      time = signalOut$time,
      temp = temp,
      par = par,
   )
   
   rModel$run()

})

windows(width = 8, height = 6)

   plot(
      signalOut$time,
      cppModelFE_output$outputDo$dox,
      pch = 2
   )
   points(
      rModel$output$time,
      rModel$output$do,
      col = "red"
   )
   points(
      signalOut$time,
      cppModelCN_output$outputDo$dox,
      pch = 0,
      col = "blue"
   )
