# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

# R6 Class CMetabOptim ####

#' @export
#'
#' @title
#'   R6 class defining an MLE inference using a metabolism model at a single location
#'
#' @description
#'   Processes a signal of DO and pCO2 data
#'   (with supporting PAR and temperature information)
#'   for inferring whole-stream metabolism using the one-station
#'   method.
#'
CMetabOptim <- R6Class(
   classname = "CMetabOptim",
   inherit = disco::SignalDerivation,
   public = list(

      ## Attributes ####

      #' @field initParams
      #'   The intial parameter values to use for the MLE algorithm
      initParams = NULL,

      #' @field fixedParams
      #'   A named list of values for non-estimated parameters
      fixedParams = NULL,

      #' @field objFunc
      #'   The objective function to use with optim for the inference
      objFunc = NULL,

      #' @field modelType
      #'   Character string representing the type of model calculation to use
      modelType = NULL,

      #' @field staticAirPressure
      #'   The air pressure.
      staticAirPressure = NULL,

      #' @field staticGwAlpha
      #'   Static value for the channel turnover rate due to inflowing groundwater
      staticGwAlpha = NULL,

      #' @field staticGwDO
      #'   Static value for DO concentration in inflowing groundwater
      staticGwDO = NULL,

      #' @field useDO
      #'   Logical flag to indicate whether DO data should be used for inference
      useDO = NULL,

      #' @field doHeader
      #'   character string representing the header for DO to use. Null
      #'   value will prevent DO from being used in analysis.
      doHeader = NULL,

      #' @field usepCO2
      #'   Logical flag to indicate whether pCO2 data should be used for inference
      usepCO2 = NULL,

      #' @field pCO2Header
      #'   Character string representing the header for pCO2 to use. Null
      #'   value will prevent pCO2 from being used in analysis.
      pCO2Header = NULL,

      #' @field dicHeader
      #'   Character string representing the header for DIC to use.
      dicHeader = NULL,

      #' @field parHeader
      #'   Character string representing the header for PAR to use.
      parHeader = NULL,

      #' @field tempHeader
      #'   Character string representing the header for temperature to use.
      tempHeader = NULL,

      #' @field airPressureHeader
      #'   Character string representing the header for air pressure to use.
      airPressureHeader = NULL,

      #' @field gwAlphaHeader
      #'   Character string representing the header for
      #'   channel turnover rate due to inflowing groundwater.
      gwAlphaHeader = NULL,

      #' @field gwDOHeader
      #'   Character string representing the header for DO in inflowing groundwater
      gwDOHeader = NULL,

      #' @field outputFile
      #'   The name of the output file.
      outputFile = NULL,

      #' @field staticCO2Air
      #'   The pCO2 in the air.
      staticCO2Air = NULL,

      #' @field staticAlkalinity
      #'   The alkalinity.
      staticAlkalinity = NULL,

      #' @field staticGwDIC
      #'   Static value for DIC in groundwater
      staticGwDIC = NULL,

      #' @field co2AirHeader
      #'   Character string representing the header for co2 partial pressure in air to use.
      co2AirHeader = NULL,

      #' @field alkalinityHeader
      #'   Character string representing the header for alkalinity to use.
      alkalinityHeader = NULL,

      #' @field gwDICHeader
      #'   Character string representing the header for DIC in groundwater
      gwDICHeader = NULL,

      #' @field optimArgs
      #'   A list representing arguments to pass to optim
      optimArgs = NULL,

      ## Method: constructor ####

      #' @description
      #'   Initializes a new object of the class.
      #'
      #' @param ...
      #'   Parameters passed to the initialize method of the super class.
      #'   See \link{CMetabOptim} for documentation on these parameters
      #' @param initParams
      #'   The initial parameter values to use in the MLE algorithm
      #' @param fixedParams
      #'   An optional named list of values for non-estimated parameters.
      #'   Defaults to NULL, which indicates no fixed parameters.
      #' @param objFunc
      #'   The objective function to use with optim for the inference
      #' @param modelType
      #'   Character string representing the type of model calculation to use
      #' @param useDO
      #'   Logical flag to indicate whether DO data should be used for inference
      #' @param doHeader
      #'   character string representing the header for DO to use. Null
      #'   value will prevent DO from being used in analysis.
      #' @param usepCO2
      #'   Logical flag to indicate whether pCO2 data should be used for inference
      #' @param pCO2Header
      #'   Character string representing the header for pCO2 to use. Null
      #'   value will prevent pCO2 from being used in analysis.
      #' @param dicHeader
      #'   Character string representing the header for DIC to use.
      #' @param parHeader
      #'   Character string representing the header for PAR to use.
      #' @param tempHeader
      #'   Character string representing the header for temperature to use.
      #' @param staticAirPressure
      #'   The air pressure.
      #' @param airPressureHeader
      #'   Character string representing the header for air pressure to use.
      #' @param outputFile
      #'   The name of the output file.
      #' @param staticCO2Air
      #'   The pCO2 in the air.
      #' @param co2AirHeader
      #'   Character string representing the header for co2 partial pressure in air to use.
      #' @param staticAlkalinity
      #'   The alkalinity.
      #' @param alkalinityHeader
      #'   Character string representing the header for alkalinity to use.
      #' @param staticGwAlpha
      #'   Static value for the channel turnover rate due to inflowing groundwater.
      #'   Default value is NULL, meaning values will be taken from the
      #'   variable specified by gwAlphaHeader in the signal.
      #' @param gwAlphaHeader
      #'   Character string representing the header for
      #'   channel turnover rate due to inflowing groundwater.
      #'   Default value is NULL.
      #'   When both static value and header are NULL, simulation of
      #'   groundwater influence is disabled.
      #' @param staticGwDO
      #'   Static value for DO concentration in inflowing groundwater.
      #'   Default value is NULL, meaning values will be taken from the
      #'   variable specified by gwDOHeader in the signal.
      #' @param gwDOHeader
      #'   Character string representing the header for DO in inflowing groundwater
      #'   Default value is NULL.
      #'   When both static value and header are NULL, simulation of
      #'   groundwater influence is disabled.
      #' @param staticGwDIC
      #'   Static value for DIC in groundwater.
      #'   Default value is NULL, meaning values will be taken from the
      #'   variable specified by gwDICHeader in the signal.
      #' @param gwDICHeader
      #'   Character string representing the header for DIC in groundwater
      #'   Default value is NULL.
      #'   When both static value and header are NULL, simulation of
      #'   groundwater influence is disabled.
      #' @param optimArgs
      #'   A list representing arguments to pass to optim
      #'
      initialize = function
      (
         ...,
         initParams,
         fixedParams = NULL,
         objFunc,
         modelType = "ForwardEuler",
         useDO,
         doHeader = "do",
         usepCO2,
         pCO2Header = "pCO2",
         dicHeader = "dic",
         parHeader = "par",
         tempHeader = "temp",
         staticAirPressure = NULL,
         airPressureHeader = "airPressure",
         outputFile = "results",
         staticCO2Air = NULL,
         co2AirHeader = "co2Air",
         staticAlkalinity = NULL,
         alkalinityHeader = "alkalinity",
         staticGwAlpha = NULL,
         gwAlphaHeader = NULL,
         staticGwDO = NULL,
         gwDOHeader = NULL,
         staticGwDIC = NULL,
         gwDICHeader = NULL,
         optimArgs = NULL
      )
      {
         super$initialize(...);

         self$initParams <- initParams;
         self$fixedParams <- fixedParams;
         self$objFunc <- objFunc;
         self$modelType <- modelType;
         self$useDO <- useDO;
         self$doHeader <- doHeader;
         self$usepCO2 <- usepCO2;
         self$pCO2Header <- pCO2Header;
         self$dicHeader <- dicHeader;
         self$parHeader <- parHeader;
         self$tempHeader <- tempHeader;
         self$staticAirPressure <- staticAirPressure;
         self$airPressureHeader <- airPressureHeader;
         self$outputFile <- outputFile;
         self$staticCO2Air <- staticCO2Air;
         self$co2AirHeader <- co2AirHeader;
         self$staticAlkalinity <- staticAlkalinity;
         self$alkalinityHeader <- alkalinityHeader;
         self$staticGwAlpha = staticGwAlpha;
         self$gwAlphaHeader = gwAlphaHeader;
         self$staticGwDO = staticGwDO;
         self$gwDOHeader = gwDOHeader;
         self$staticGwDIC = staticGwDIC;
         self$gwDICHeader = gwDICHeader;
         self$optimArgs = optimArgs;
      },

      ## Method: derive ####

      #' @description
      #'   Performs a metabolism analysis using the minimization of a
      #'   value from a provided objective function.
      #'
      #' @param signal
      #'   The signal with the DO or pCO2 on which the metabolism estimate is based.
      #'   If null, the signal attribute will be used.
      #' @param prevResults
      #'   If available, the results from the previous window of analysis.
      #' @param path
      #'   The path to the location where the output file is written
      #' @param index
      #'   The index of the row of the data frame to use for any fixed
      #'   parameter values.
      #'
      #' @return
      #'   The results object from the analysis
      #'
      derive = function
      (
         signal = NULL,
         prevResults = NULL,
         path,
         index
      )
      {

         dailyGPP <- self$initParams["dailyGPP"];
         dailyER <- self$initParams["dailyER"];
         k600 <- self$initParams["k600"];
         if(!is.null(self$fixedParams)) {
            if(!is.null(self$fixedParams$dailyGPP)) {
               dailyGPP <- self$fixedParams$dailyGPP[index];
            }
            if(!is.null(self$fixedParams$dailyER)) {
               dailyER <- self$fixedParams$dailyER[index];
            }
            if(!is.null(self$fixedParams$k600)) {
               k600 <- self$fixedParams$k600[index];
            }
         }

         if(!is.null(signal)) {
            self$signal <- signal;
         } else {
            if(is.null(self$signal)) {
               stop(paste(
                  "Signal not provided to SignalDerivation$derive."
               ));
            }
         }

         if (!is.null(self$staticAirPressure)) {
            airPressure <- self$staticAirPressure;
         } else {
            airPressure <- self$signal$getVariable(self$airPressureHeader);
         }

         gwDODisabled <-
            (is.null(self$staticGwAlpha) && is.null(self$gwAlphaHeader)) ||
            (is.null(self$staticGwDO) && is.null(self$gwDOHeader));
         if (gwDODisabled) {
            gwAlpha <- NA;
            gwDO <- NA;
         } else {
            if (!is.null(self$staticGwAlpha)) {
               gwAlpha <-
                  rep(x = 0 , times = self$signalOut$getLength()) +
                  self$staticGwAlpha;
            } else {
               gwAlpha <- self$signalOut$getVariable(self$gwAlphaHeader);
            }
            if (!is.null(self$staticGwDO)) {
               gwDO <-
                  rep(x = 0 , times = self$signalOut$getLength()) +
                  self$staticGwDO;
            } else {
               gwDO <- self$signalOut$getVariable(self$gwDOHeader);
            }
         }

         if(self$usepCO2) {

            if (!is.null(self$staticCO2Air)) {
               co2Air <- self$staticCO2Air;
            } else {
               co2Air <- self$signal$getVariable(self$co2AirHeader);
            }

            if (!is.null(self$staticAlkalinity)) {
               alkalinity <- self$staticAlkalinity;
            } else {
               alkalinity <- self$signal$getVariable(self$alkalinityHeader);
            }

            gwDICDisabled <-
               gwDODisabled ||
               (is.null(self$staticGwDIC) && is.null(self$gwDICHeader));
            if (gwDICDisabled)
            {
               gwDIC <- NA;
            } else {
               if (!is.null(self$staticGwDIC)) {
                  gwDIC <-
                     rep(x = 0 , times = self$signalOut$getLength()) +
                     self$staticGwDIC;
               } else {
                  gwDIC <- self$signalOut$getVariable(self$gwDICHeader);
               }
            }
         }

         if(!self$usepCO2) {
            model <- CMetabDo$new(
               type = self$modelType,
               dailyGPP = dailyGPP,
               dailyER = dailyER,
               k600 = k600,
               initialDO = self$signal$getVariable(self$doHeader)[1],
               time = self$signal$getTime(),
               temp = self$signal$getVariable(self$tempHeader),
               par = self$signal$getVariable(self$parHeader),
               airPressure = airPressure,
               stdAirPressure = 1,
               gwAlpha = gwAlpha,
               gwDO = gwDO
            );
         } else {
            dicobs <- self$signal$getVariable(self$dicHeader);
            dicobs <- dicobs[is.finite(dicobs)];
            model <- CMetabDoDic$new(
               type = self$modelType,
               dailyGPP = dailyGPP,
               dailyER = dailyER,
               k600 = k600,
               initialDO = self$signal$getVariable(self$doHeader)[1],
               time = self$signal$getTime(),
               temp = self$signal$getVariable(self$tempHeader),
               par = self$signal$getVariable(self$parHeader),
               airPressure = airPressure,
               stdAirPressure = 1,
               gwAlpha = gwAlpha,
               gwDO = gwDO,
               initialDIC = dicobs[1],
               pCO2air = co2Air,
               alkalinity = alkalinity,
               gwDIC = gwDIC
            );
         }

         self$objFunc$setModel(model = model);

         if (self$useDO) {
            observation <- data.frame(do = self$signal$getVariable(self$doHeader));
            if (self$usepCO2) {
               observation$pCO2 <- self$signal$getVariable(self$pCO2Header);
            }
         } else if (self$usepCO2) {
            observation <- data.frame(pCO2 = self$signal$getVariable(self$pCO2Header));
         } else {
            stop("Need to use at least one of DO or pCO2 to perform optimization.");
         }

         self$objFunc$setObservation(observation = observation);

         if (is.null(prevResults)) {
            par <- self$initParams;
         } else {
            par <- prevResults$optimr$par;
         }

         args <- c(
            list(
               par = par,
               fn = self$objFunc$propose
            ),
            self$optimArgs
         )
         optimr <- do.call(
            what = optim,
            args = args
         );
         self$objFunc$propose(optimr$par);
         results <- list(
            params = optimr$par,
            time = self$signal$getTime(),
            temp = self$signal$getVariable(self$tempHeader),
            par = self$signal$getVariable(self$parHeader),
            pred = model$output,
            objFuncMultivariateValues = self$objFunc$multivariateValues,
            objFuncValue = self$objFunc$value,
            optimr = optimr
         );

         saveRDS(
            results,
            file = sprintf(
               fmt = "%s/%s.RData",
               path,
               self$outputFile
            )
         );
         return(results);
      }
   )
)
