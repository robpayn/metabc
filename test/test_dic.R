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
      "MetabForwardEulerDoDic_constructor",
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
      760,
      -1,
      1,
      signalOut$getVariable("dic")[1],
      rep(400, length(numTime)),
      rep(2857, length(numTime))
   )
   
   cppModelFE_output <- .Call("MetabDoDic_run", cppModelFE$baseExternalPointer);
   
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
      "MetabCrankNicolsonDoDic_constructor",
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
      760,
      -1,
      1,
      signalOut$getVariable("dic")[1],
      rep(400, length(numTime)),
      rep(2857, length(numTime))
   )
   
   cppModelCN_output <- .Call("MetabDoDic_run", cppModelCN$baseExternalPointer);
   
})

timers$rTime <- bench_time({
   
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

})

windows(width = 8, height = 11)

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
