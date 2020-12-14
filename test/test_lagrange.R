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

timers <- list(cppTimeFE = bench_time({
   
   cppLagrangeCN <- .Call("MetabLagrangeCNOneStepDo_constructor")
   .Call(
      "MetabLagrangeDo_initialize",
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
      0
   )
   cppLagrangeCN_output <- .Call("MetabLagrangeDo_run", cppLagrangeCN$baseExternalPointer);
   
}))

timers$rTime <- bench_time({
   
   rModel <- TwoStationMetabDo$new(
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
      stdAirPressure = stdAirPressure
   )
   rModel$run()
   
})   

windows(width = 8, height = 10)

   par(mfrow = c(2, 1), mar = c(4.5, 4.5, 1, 1))
   
   plot(
      signalOut$time,
      cppLagrangeCN_output$outputDo$dox,
      pch = 2
   )
   points(
      rModel$output$time,
      rModel$output$do,
      col = "red"
   )

   plot(
      signalOut$time,
      cppLagrangeCN_output$outputDo$dox - upstreamDO,
      pch = 2
   )
   points(
      rModel$output$time,
      rModel$output$do - upstreamDO,
      col = "red"
   )
