# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @importFrom metab OneStationMetabDo
#' @useDynLib metabc

# Class definition ####

#' @export
#'
#' @title
#'   Construct a one-station DO model object
#' @return
#'   Pointer to an instance of a one-station DO model object
#'
CppOneStationMetabDo <- R6Class(
   classname = "CppOneStationMetabDo",
   inherit = OneStationMetabDo,
   public = list(

      CModel = NULL,

      initialize = function(..., time, airPressure, stdAirPressure = 760)
      {
         super$initialize(
            ...,
            time = time,
            airPressure = airPressure,
            stdAirPressure = stdAirPressure
         );

         if (length(airPressure) == 1) {
            self$airPressure = rep(x = airPressure[1], times = length(time));
         } else if (length(airPressure) == length(time)) {
            self$airPressure = airPressure;
         } else {
            stop("Length of air pressure vector must be 1 or the same length as the time vector.")
         }

         self$CModel <- .Call(
            "MetabForwardEulerDo_constructor",
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
            PACKAGE = "metabc"
         );

         self$dt <- .Call(
            "MetabForwardEulerDo_getDt",
            self$CModel,
            PACKAGE = "metabc"
         )
      },

      finalize = function()
      {
         .Call(
            "MetabForwardEulerDo_destructor",
            self$CModel,
            PACKAGE = "metabc"
         )
      },

      run = function()
      {
         result <- .Call(
            "MetabForwardEulerDo_run",
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
            dt = self$dt
         )
         return(self$output)
      },

      setDailyGPP = function(dailyGPP)
      {
         self$dailyGPP = dailyGPP;
         .Call(
            "MetabForwardEulerDo_setDailyGPP",
            self$CModel,
            dailyGPP,
            PACKAGE = "metabc"
         )
      }
   )
)

