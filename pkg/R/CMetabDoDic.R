# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @useDynLib metabc

# Class CMatabDo ####

#' @export
#'
#' @title
#'   Time series DO and DIC metabolism model based on C++ implementation
#'
#' @description
#'   Predicts the change in dissolved oxygen and dissolved inorganic carbon
#'   in an aquatic system over time due to metabolic processes and
#'   gas exchange with the atmosphere.
#'
#'   Implementation of this model is provided by C++ objects.
#'
CMetabDoDic <- R6Class(
   classname = "CMetabDoDic",
   inherit = CMetab,
   public = list(

      # Class CMatabDo: Attributes ####

      #' @field type
      #'   A character string indicating type of solution used.
      #'   (e.g. "ForwardEuler" or "CrankNicolson" are known solutions)
      type = NULL,

      #' @field time
      #'   A numeric vector of time in units of days representing the
      #'   times at which DO concentrations are simulated
      time = NULL,

      #' @field timePOSIX
      #'   A vector of POSIXct type time objects representing the
      #'   times at which DO concentrations are simulated
      timePOSIX = NULL,

      #' @field airPressure
      #'   A vector of barometric pressures at the times when DO
      #'   values are to be simulated
      airPressure = NULL,

      #' @field output
      #'   The output generated when the model is run.
      #'   This is a list of dataframes.
      output = NULL,

      #' @field pCO2air
      #'   A vector of partial pressures of CO2 at the times when DO
      #'   values are to be simulated
      pCO2air = NULL,

      #' @field alkalinity
      #'   A vector of alkalinities due to carbonate at the times when DO
      #'   values are to be simulated
      alkalinity = NULL,

      # Class CMatabDo: Methods ####

      #' @description
      #'   Constructs an object that is a new instance of the class
      #'
      #' @param type
      #'    A character string indicating type of solution used.
      #'    (e.g. "ForwardEuler" or "CrankNicolson" are known solutions)
      #' @param dailyGPP
      #'    Model parameter for daily gross primary production
      #'    based on an effective concentration of DOC fixed.
      #'    Units of micromoles per liter per day.
      #' @param ratioDoCFix
      #'    Ratio of DO molecules produced relative to carbon atoms fixed.
      #'    Defaults to 1.
      #' @param dailyER
      #'    Model parameter for daily aerobic ecosystem respiration
      #'    based on an effective concentration of DOC respired
      #'    Units of micromoles per liter per day.
      #' @param ratioDoCResp
      #'    Ratio of DO molecules consumed relative to carbon atoms respired.
      #'    Defaults to -1.
      #' @param k600
      #'    Influence of atmospheric gas exchange on oxygen concentration
      #'    as a first-order rate depending on saturation deficit.
      #'    Units are per day.
      #' @param initialDO
      #'    Initial DO concentration at the beginning of the evaluation period.
      #'    Units are micromoles O2 per liter.
      #' @param time
      #'    Vector of times at which DO will be predicted.
      #'    These times must align with vectors provided for temperature and
      #'    photosynthetically active radiation.
      #'    Type should be a POSIXct vector or character vector that can be forced
      #'    to a POSIXct by default (e.g. format of "YYYY-MM-DD HH:mm:ss").
      #' @param temp
      #'    Vector of water temperatures in degrees Celsius.
      #'    Must be same length as the time vector.
      #' @param par
      #'    Vector of photosynthetically active radiation values.
      #'    Units are arbitrary if linear with PAR intensity, but must be consistent
      #'    with total PAR if the optional argument for total PAR is specified.
      #'    Must be the same length as the time vector.
      #' @param parTotal
      #'    Total PAR for period of analysis, see units note in par argument description.
      #'    If a negative value is provided, the model will integrate the par data
      #'    provided to estimate the total PAR.
      #'    Defaults to -1.
      #' @param airPressure
      #'    Barometric pressure.
      #'    If a single value is provided, a vector of the length of the time vector
      #'    will be created from that value.
      #'    Units must be consistent with the units of the optional standard air pressure
      #'    argument.
      #'    If default stdAirPressure is used, this should be a fraction of the standard
      #'    air pressure at sea level.
      #' @param stdAirPressure
      #'    The standard air pressure in the desired units.
      #'    Defaults to 1 standard atmosphere.
      #' @param ratioDicCFix
      #'    Ratio of DIC-C atoms consumed relative to organic carbon atoms fixed.
      #'    Defaults to -1.
      #' @param ratioDicCResp
      #'    Ratio of DIC-C atoms produced relative to organic carbon atoms respired
      #'    Defaults to 1.
      #' @param initialDIC
      #'    Initial DIC concentration at the beginning of the evaluation period.
      #'    Units are micromoles C per liter.
      #' @param pCO2air
      #'   A vector of partial pressures of CO2 at the times when DO
      #'   values are to be simulated.
      #'   If a single value is provided, a vector of the length of the time vector
      #'   will be created from that value.
      #' @param alkalinity
      #'   A vector of alkalinities due to carbonate at the times when DO
      #'   values are to be simulated.
      #'   If a single value is provided, a vector of the length of the time vector
      #'   will be created from that value.
      #'
      initialize = function
      (
         type = "ForwardEuler",
         dailyGPP,
         ratioDoCFix = 1,
         dailyER,
         ratioDoCResp = -1,
         k600,
         initialDO,
         time,
         temp,
         par,
         parTotal = -1,
         airPressure,
         stdAirPressure = 1,
         ratioDicCFix = -1,
         ratioDicCResp = 1,
         initialDIC,
         pCO2air,
         alkalinity
      )
      {
         self$type <- type;
         self$timePOSIX <- as.POSIXct(time);
         self$time <- as.numeric(time) / 86400;

         self$airPressure <- rep(0, length(time)) + airPressure;

         self$pCO2air <- rep(0, length(time)) + pCO2air;
         self$alkalinity <- rep(0, length(time)) + alkalinity;

         self$pointers <- .Call(
            sprintf("Metab%sDoDic_constructor", self$type)
         );
         .Call(
            "MetabDoDic_initialize",
            self$pointers$baseExternalPointer,
            dailyGPP,
            ratioDoCFix,
            dailyER,
            ratioDoCResp,
            k600,
            initialDO,
            self$time,
            temp,
            par,
            parTotal,
            self$airPressure,
            stdAirPressure,
            ratioDicCFix,
            ratioDicCResp,
            initialDIC,
            self$pCO2air,
            self$alkalinity
         )
      },

      #' @description
      #'   Calls the R wrapper of the destructor of the
      #'   underlying C object to release memory.
      #'   Do not call this manually unless you really know what you are doing.
      #'
      #' @return
      #'   SEXP object returned by the destructor method
      #'
      finalize = function()
      {
         .Call(
            sprintf("Metab%sDoDic_destructor", self$type),
            self$pointers$modelExternalPointer
         )
      },

      #' @description
      #'   Runs a simulation based on parameters and driving data
      #'   in the object attributes. Output will be stored in the
      #'   output attribute, but is also returned from this function
      #'   as a convenience.
      #'
      #' @return
      #'   A named list of dataframes containing model output.
      #'   \itemize{
      #'     \item base: fundamental model output
      #'     \item do: model output associated with dissolved oxygen calculations
      #'     \item dic: model output associated with dissolved inorganic carbon calculations
      #'   }
      #'
      run = function()
      {
         super$run();
         results <- .Call(
            "MetabDoDic_getSummary",
            self$pointers$baseExternalPointer
         )
         self$output <- list(
            base = data.frame(results$output),
            do = data.frame(results$outputDo),
            dic = data.frame(results$outputDic)
         );
         return(self$output);
      },

      #' @description
      #'   Sets one of the DO or DIC related model parameters to a new value.
      #'
      #' @param param
      #'   A character string to indicate the parameter to be changed
      #'   \itemize{
      #'     \item "RatioDoCFix": change the DO C fixation stoichiometric parameter
      #'     \item "RatioDoCResp": change the DO C respiration stoichiometric parameter
      #'     \item "RatioDicCFix": change the DIC C fixation stoichiometric parameter
      #'     \item "RatioDicCResp": change the DIC C respiration stoichiometric parameter
      #'   }
      #' @param value
      #'   The new value for the parameter
      #'
      #' @return
      #'   The previous value of the parameter
      #'
      setMetabDoDicParam = function(param, value)
      {
         .Call(
            sprintf("MetabDoDic_set%s", param),
            self$pointers$baseExternalPointer,
            value
         )

      }
   )
)
