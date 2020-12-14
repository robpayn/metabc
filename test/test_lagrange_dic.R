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
upstreamDO <- signalIn$getVariable("do");
upstreamTime <- as.numeric(signalIn$time) / 86400
downstreamTime <- as.numeric(signalOut$time) / 86400
upstreamTemp <- signalIn$getVariable("temp")
downstreamTemp <- signalOut$getVariable("temp")
upstreamPAR <- signalIn$getVariable("par")
downstreamPAR <- signalOut$getVariable("par")
parTotal <- -1
airPressure <- rep(638, length)
stdAirPressure <- 760

gppdic <- -1
erdic <- 1
upstreamDIC <- signalOut$getVariable("dic")
upstreamDIC[15:17] <- 2846.699
pCO2Air <- rep(400, length);
upstreamAlkalinity <- rep(2857, length);
downstreamAlkalinity <- rep(2857, length);

timers <- list(cppTimeCN = bench_time({
   
   cppLagrangeCN <- .Call("MetabLagrangeCNOneStepDoDic_constructor")
   .Call(
      "MetabLagrangeDoDic_initialize",
      cppLagrangeCN$baseExternalPointer,
      gpp,
      gppdo,
      er,
      erdo,
      k600,
      upstreamDO,
      upstreamTime,
      downstreamTime,
      upstreamTemp,
      downstreamTemp,
      upstreamPAR,
      downstreamPAR,
      parTotal,
      airPressure,
      stdAirPressure,
      0,
      gppdic,
      erdic,
      upstreamDIC,
      pCO2Air,
      upstreamAlkalinity,
      downstreamAlkalinity
   )
   cppLagrangeCN_output <- .Call(
      "MetabLagrangeDoDic_run", 
      cppLagrangeCN$baseExternalPointer
   );
   
}))

timers$rTime <- bench_time({
   
   rModel <- TwoStationMetabDoDic$new(
      dailyGPP = gpp,
      ratioDoCfix = gppdo,
      dailyER = er,
      ratioDoCresp = erdo,
      k600 = k600,
      airPressure = airPressure[1],
      upstreamTime = signalIn$time,
      upstreamTemp = upstreamTemp,
      upstreamPAR = upstreamPAR,
      upstreamDO = upstreamDO,
      downstreamTime = signalOut$time,
      downstreamTemp = downstreamTemp,
      downstreamPAR = downstreamPAR,
      stdAirPressure = stdAirPressure,
      ratioDicCfix = gppdic,
      ratioDicCresp = erdic,
      upstreamDIC = upstreamDIC,
      pCO2air = pCO2Air[1],
      alkalinity = upstreamAlkalinity[1]
   )
   rModel$run()
   
})   

windows(width = 12, height = 10)

par(mfrow = c(2, 2), mar = c(4.5, 4.5, 1, 1))

plot(
   signalOut$time,
   cppLagrangeCN_output$outputDic$pCO2,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$pCO2,
   col = "red"
)

plot(
   signalOut$time,
   cppLagrangeCN_output$outputDic$dic,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$dic,
   col = "red"
)

plot(
   signalOut$time,
   cppLagrangeCN_output$outputDic$pCO2 - signalIn$getVariable("pCO2"),
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$pCO2 - signalIn$getVariable("pCO2"),
   col = "red"
)

plot(
   signalOut$time,
   cppLagrangeCN_output$outputDic$dic - upstreamDIC,
   pch = 2
)
points(
   rModel$output$time,
   rModel$output$dic - upstreamDIC,
   col = "red"
)
