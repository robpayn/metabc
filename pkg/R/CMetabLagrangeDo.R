# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @useDynLib metabc

# Class CMatabLagrangeDo ####

#' @export
#'
#' @title
#'   Lagrangian parcel DO metabolism model based on C++ implementation
#'
#' @description
#'   Predicts the change in dissolved oxygen
#'   in a parcel of water passing through an aquatic ecosystem due to
#'   metabolic processes and gas exchange with the atmosphere.
#'
#'   Implementation of this model is provided by C++ objects.
#'
CMetabLagrangeDo <- R6Class(
   classname = "CMetabLagrangeDo",
   inherit = CMetab,
   public = list(

      # Class CMatabLagrangeDo: attributes ####

      #' @field type
      #'   A character string indicating type of solution used.
      #'   (e.g. "CNOneStep" is a known solution using a single
      #'   calculation with Crank Nicolson type approximations)
      type = NULL,

      #' @field upstreamTimePOSIX
      #'   A vector of POSIX objects representing the times the
      #'   parcels passed the upstream end of the system
      upstreamTimePOSIX = NULL,

      #' @field downstreamTimePOSIX
      #'   A vector of POSIX objects representing the times the
      #'   parcels passed the downstream end of the system
      downstreamTimePOSIX = NULL,

      #' @field output
      #'   The output attribute is populated when the model is run.
      #'   This is a list of dataframes.
      output = NULL,

      # Class CMatabLagrangeDo: methods ####

      #' @description
      #'   Constructs an object that is a new instance of the class
      #'
      #' @param type
      #'    A character string indicating type of solution used.
      #'    (e.g. "CNOneStep" is a known solution using a single
      #'    calculation with Crank Nicolson type approximations)
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
      #' @param upstreamDO
      #'    Vector of DO concentrations when the parcels of water are passing
      #'    the upstream end of the system
      #' @param upstreamTime
      #'    Vector of times when parcels are passing upstream
      #'    These times must align with vectors provided for temperature and
      #'    photosynthetically active radiation.
      #'    Type should be a POSIXct vector or character vector that can be forced
      #'    to a POSIXct by default (e.g. format of "YYYY-MM-DD HH:mm:ss").
      #' @param downstreamTime
      #'    Vector of times when parcels are passing downstream
      #'    These times must align with vectors provided for temperature and
      #'    photosynthetically active radiation.
      #'    Type should be a POSIXct vector or character vector that can be forced
      #'    to a POSIXct by default (e.g. format of "YYYY-MM-DD HH:mm:ss").
      #' @param upstreamTemp
      #'    Vector of water temperatures in parcels passing upstream.
      #'    Must be same length as the time vectors.
      #' @param downstreamTemp
      #'    Vector of water temperatures in parcels passing downstream.
      #'    Must be same length as the time vectors.
      #' @param upstreamPAR
      #'    Vector of photosynthetically active radiation values on parcels
      #'    passing upstream.
      #'    Units are arbitrary if linear with PAR intensity, but must be consistent
      #'    with total PAR if the optional argument for total PAR is specified.
      #'    Must be the same length as the time vector.
      #' @param downstreamPAR
      #'    Vector of photosynthetically active radiation values on parcels
      #'    passing downstream.
      #'    Units are arbitrary if linear with PAR intensity, but must be consistent
      #'    with total PAR if the optional argument for total PAR is specified.
      #'    Must be the same length as the time vector.
      #' @param parTotal
      #'    Total PAR for period of analysis, see units note in par argument description.
      #'    If a negative value is provided, the model will integrate the par data
      #'    provided to estimate the total PAR.
      #'    Defaults to -1.
      #' @param airPressure
      #'    Barometric pressure. A vector for the parcels can be provided, or a
      #'    single value will be automatically turned into a
      #'    vector of values for the parcels.
      #'    Units must be consistent with the units of the optional standard air pressure
      #'    argument.
      #'    If default stdAirPressure is used, this should be a fraction of the standard
      #'    air pressure at sea level.
      #' @param stdAirPressure
      #'    The standard air pressure in the desired units.
      #'    Defaults to 1 standard atmosphere.
      #' @param timesteps
      #'    The number of timesteps for calculations for each parcel.
      #'    Note that certain calculation algorithms (e.g. "OneStep" calculators)
      #'    will ignore this argument.
      #'    Defaults to 2 (a single calculation per parcel).
      #'
      initialize = function
      (
         type = "CNOneStep",
         dailyGPP,
         ratioDoCFix = 1,
         dailyER,
         ratioDoCResp = -1,
         k600,
         upstreamDO,
         upstreamTime,
         downstreamTime,
         upstreamTemp,
         downstreamTemp,
         upstreamPAR,
         downstreamPAR,
         parTotal = -1,
         airPressure,
         stdAirPressure = 1,
         timesteps = 2
      )
      {
         self$type <- type;
         self$upstreamTimePOSIX <- as.POSIXct(upstreamTime);
         self$downstreamTimePOSIX <- as.POSIXct(downstreamTime);
         upstreamTime <- as.numeric(upstreamTime) / 86400;
         downstreamTime <- as.numeric(downstreamTime) / 86400;
         airPressure <- rep(0, length(upstreamTime)) + airPressure;

         self$pointers <- .Call(
            sprintf("MetabLagrange%sDo_constructor", self$type)
         );
         .Call(
            "MetabLagrangeDo_initialize",
            self$pointers$baseExternalPointer,
            dailyGPP,
            ratioDoCFix,
            dailyER,
            ratioDoCResp,
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
            timesteps
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
            sprintf("MetabLagrange%sDo_destructor", self$type),
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
      #'   }
      #'
      run = function()
      {
         super$run();
         results <- .Call(
            "MetabLagrangeDo_getSummary",
            self$pointers$baseExternalPointer
         )
         self$output <- list(
            base = data.frame(results$output),
            do = data.frame(results$outputDo)
         );
         return(self$output);
      },

      #' @description
      #'   Sets one of the DO related model parameters to a new value.
      #'
      #' @param param
      #'   A character string to indicate the parameter to be changed
      #'   \itemize{
      #'     \item "RatioDoCFix": change the C fixation stoichiometric parameter
      #'     \item "RatioDoCResp": change the C respiration stoichiometric parameter
      #'   }
      #' @param value
      #'   The new value for the parameter
      #'
      #' @return
      #'   The previous value of the parameter
      #'
      setMetabLagrangeDoParam = function(param, value)
      {
         .Call(
            sprintf("MetabLagrangeDo_set%s", param),
            self$pointers$baseExternalPointer,
            value
         )
      }
   )
)
