# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @importFrom metab OneStationMetabDoDic
#' @useDynLib metabc

# Class definition ####

#' @export
#'
#' @title
#'   Construct a one-station DO model object
#' @return
#'   Pointer to an instance of a one-station DO model object
#'
CppOneStationMetabDoDic <- R6Class(
   classname = "CppOneStationMetabDoDic",
   inherit = OneStationMetabDoDic,
   public = list(
      CModel = NULL,

      initialize = function
      (
         ...,
         time,
         airPressure,
         stdAirPressure = 760,
         pCO2air,
         alkalinity
      )
      {
         super$initialize(
            ...,
            time = time,
            airPressure = airPressure,
            stdAirPressure = stdAirPressure,
            pCO2air = pCO2air,
            alkalinity = alkalinity
         );

         if (length(airPressure) == 1) {
            self$airPressure = rep(x = airPressure[1], times = length(time));
         } else if (length(airPressure) == length(time)) {
            self$airPressure = airPressure;
         } else {
            stop("Length of air pressure vector must be 1 or the same length as the time vector.")
         }

         if (length(pCO2air) == 1) {
            self$pCO2air = rep(x = pCO2air[1], times = length(time));
         } else if (length(pCO2air) == length(time)) {
            self$pCO2air = pCO2air;
         } else {
            stop("Length of pCO2air vector must be 1 or the same length as the time vector.")
         }

         if (length(alkalinity) == 1) {
            self$alkalinity = rep(x = alkalinity[1], times = length(time));
         } else if (length(alkalinity) == length(time)) {
            self$alkalinity = alkalinity;
         } else {
            stop("Length of alkalinity vector must be 1 or the same length as the time vector.")
         }

         self$CModel <- .Call(
            "MetabForwardEulerDoDic_constructor",
            self$dailyGPP,
            self$ratioDoCfix,
            self$dailyER,
            self$ratioDoCresp,
            self$k600,
            self$initialDO,
            self$time,
            self$temp,
            self$par,
            self$parTotal,
            self$airPressure,
            stdAirPressure,
            self$ratioDicCfix,
            self$ratioDicCresp,
            self$initialDIC,
            self$pCO2air,
            self$alkalinity,
            PACKAGE = "metabc"
         );

         self$dt <- .Call(
            "MetabForwardEulerDoDic_getDt",
            self$CModel,
            PACKAGE = "metabc"
         )
      },

      finalize = function()
      {
         .Call(
            "MetabForwardEulerDoDic_destructor",
            self$CModel,
            PACKAGE = "metabc"
         )
      },

      run = function()
      {
         result <- .Call(
            "MetabForwardEulerDoDic_run",
            self$CModel,
            PACKAGE = "metabc"
         )
         self$output <- data.frame(
            time = self$timePOSIX,
            do = result[[2]][[1]],
            doSat = result[[2]][[2]],
            cFixation = result[[1]][[1]],
            cRespiration = result[[1]][[2]],
            doProduction = result[[2]][[3]],
            doConsumption = result[[2]][[4]],
            k = result[[2]][[5]],
            doEquilibration = result[[2]][[6]],
            temp = self$temp,
            dt = self$dt,
            pCO2 = result[[3]][[1]],
            dic = result[[3]][[2]],
            dicProduction = result[[3]][[3]],
            dicConsumption = result[[3]][[4]],
            co2Equilibration = result[[3]][[5]],
            kCO2 = result[[3]][[6]],
            pH = result[[3]][[7]],
            kH = result[[3]][[8]]
         )
         return(self$output)
      }
   )
)

