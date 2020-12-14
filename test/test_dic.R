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

gppdic <- -1
erdic <- 1
initDIC <- signalOut$getVariable("dic")[1]
pCO2Air <- rep(400, length);
alkalinity <- rep(2857, length);

timers <- list(cppTimeFE = bench_time({
   
   cppModelFE <- .Call("MetabForwardEulerDoDic_constructor")
   .Call(
      "MetabDoDic_initialize",
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
      stdAirPressure,
      gppdic,
      erdic,
      initDIC,
      pCO2Air,
      alkalinity
   )
   
   cppModelFE_output <- .Call("MetabDoDic_run", cppModelFE$baseExternalPointer);
   
}))

timers$cppTimeCN <- bench_time({
   
   cppModelCN <- .Call("MetabCrankNicolsonDoDic_constructor")
   .Call(
      "MetabDoDic_initialize",
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
      stdAirPressure,
      gppdic,
      erdic,
      initDIC,
      pCO2Air,
      alkalinity
   )
   
   cppModelCN_output <- .Call("MetabDoDic_run", cppModelCN$baseExternalPointer);
   
})

timers$rTime <- bench_time({
   
   rModel <- OneStationMetabDoDic$new(
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
      ratioDicCfix = gppdic,
      ratioDicCresp = erdic,
      initialDIC = initDIC,
      pCO2air = pCO2Air[1],
      alkalinity = alkalinity[1],
   )
   
   rModel$run()

})

windows(width = 8, height = 10)

   par(mfrow = c(2, 1), mar = c(4.5, 4.5, 1, 1))
   
   plot(
      signalOut$time,
      cppModelFE_output$outputDic$pCO2,
      pch = 2
   )
   points(
      rModel$output$time,
      rModel$output$pCO2,
      col = "red"
   )
   points(
      signalOut$time,
      cppModelCN_output$outputDic$pCO2,
      pch = 0,
      col = "blue"
   )
   
   plot(
      signalOut$time,
      cppModelFE_output$outputDic$dic,
      pch = 2
   )
   points(
      rModel$output$time,
      rModel$output$dic,
      col = "red"
   )
   points(
      signalOut$time,
      cppModelCN_output$outputDic$dic,
      pch = 0,
      col = "blue"
   )
