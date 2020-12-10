rm(list = ls())

library(metab)
library(metabc)
library(bench)

load(file = "./2014-09-13/signal.RData")

timers <- list(cppTimeFE = bench_time({
   
   numTime <- as.numeric(signalOut$time) / 86400
   dt <- c(
      numTime[2:length(numTime)] - numTime[1:(length(numTime) - 1)],
      0
   );
   
   par <- signalOut$getVariable("par");
   parAverage <- c(
      0.5 * (par[1:length(numTime) - 1] + par[2:length(numTime)]),
      0
   );
   parDist <- (par * dt) / sum(parAverage * dt);
   
   cppModelFE <- .Call(
      "MetabForwardEulerDo_constructor",
      610,
      1,
      275,
      -1,
      19.5,
      signalOut$getVariable("do")[1],
      numTime,
      signalOut$getVariable("temp"),
      parDist,
      rep(638, length(numTime)),
      760
   )
   
   cppModelFE_output <- .Call("MetabDo_run", cppModelFE$baseExternalPointer);

}))

timers$cppTimeCN <- bench_time({

   numTime <- as.numeric(signalOut$time) / 86400
   dt <- c(
      numTime[2:length(numTime)] - numTime[1:(length(numTime) - 1)],
      0
   );
   
   par <- signalOut$getVariable("par");
   parAverage <- c(
      0.5 * (par[1:length(numTime) - 1] + par[2:length(numTime)]),
      0
   );
   parDist <- (par * dt) / sum(parAverage * dt);
   
   cppModelCN <- .Call(
      "MetabCrankNicolsonDo_constructor",
      610,
      1,
      275,
      -1,
      19.5,
      signalOut$getVariable("do")[1],
      numTime,
      signalOut$getVariable("temp"),
      parDist,
      rep(638, length(numTime)),
      760
   )
   
   cppModelCN_output <- .Call("MetabDo_run", cppModelCN$baseExternalPointer)

})

timers$rTime <- bench_time({
   
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
