# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

#' @export
#'
#' @title
#'   R6 class defining an MLE inference using a two station metabolism model
#'
#' @description
#'   Basic MLE inference for two station stream metabolism
#'
CMetabLagrangeOptim <- R6Class(
   classname = "CMetabLagrangeOptim",
   inherit = disco::TransferFunctionDerivation,
   public = list(

      #' @field initParams
      #'   The intial parameter values to use for the MLE algorithm
      initParams = NULL,

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

      #' @field staticGwpCO2
      #'   Static value for pCO2 groundwater
      staticGwpCO2 = NULL,

      #' @field co2AirHeader
      #'   Character string representing the header for co2 partial pressure in air to use.
      co2AirHeader = NULL,

      #' @field alkalinityHeader
      #'   Character string representing the header for alkalinity to use.
      alkalinityHeader = NULL,

      #' @field gwpCO2Header
      #'   Character string representing the header for pCO2 groundwater
      gwpCO2Header = NULL,

      #' @field optimArgs
      #'   A list representing arguments to pass to optim
      optimArgs = NULL,


      #' @description
      #'   Initializes a new object of the class.
      #'
      #' @param ...
      #'   Parameters passed to the initialize method of the super class.
      #'   See \link{TransferFunctionDerivation} for documentation on these parameters
      #' @param initParams
      #'   The intial parameter values to use for the MLE algorithm
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
      #'   Static value for the channel turnover rate due to inflowing groundwater
      #' @param gwAlphaHeader
      #'   Character string representing the header for
      #'   channel turnover rate due to inflowing groundwater.
      #' @param staticGwDO
      #'   Static value for DO concentration in inflowing groundwater
      #' @param gwDOHeader
      #'   Character string representing the header for DO in inflowing groundwater
      #' @param staticGwpCO2
      #'   Static value for pCO2 groundwater
      #' @param gwpCO2Header
      #'   Character string representing the header for pCO2 groundwater
      #' @param optimArgs
      #'   A list representing arguments to pass to optim
      #'
      initialize = function
      (
         ...,
         initParams,
         objFunc,
         modelType = "CNOneStep",
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
         gwAlphaHeader = "gwAlpha",
         staticGwDO = NULL,
         gwDOHeader = "gwDO",
         staticGwpCO2 = NULL,
         gwpCO2Header = "gwpCO2",
         optimArgs = NULL
      )
      {
         super$initialize(...);

         self$initParams <- initParams;
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
         self$staticGwpCO2 = staticGwpCO2;
         self$gwpCO2Header = gwpCO2Header;
         self$optimArgs = optimArgs;
      },

      # Method TwoStationMetabMLE$derive ####
      #
      #' @description
      #'   Uses a two-station model to infer whole-stream metabolism and
      #'   gas exchange parameters from an upstream and downstream signal.
      #'
      #' @param signalIn
      #'   The input (upstream) signal on which the MLE inference is based
      #' @param signalOut
      #'   The output (downstream) signal on which the MLE inference is based
      #' @param prevResults
      #'   The results from the previous inference
      #' @param path
      #'   The path to where the results should be written
      #'
      #' @return
      #'   The two-element list containint the results of the MLE.
      #'   \itemize{
      #'     \item objFunc: The objective function used for the MLE
      #'     \item optimr: The results from optim performing the MLE
      #'   }
      #'
      derive = function
      (
         signalIn = NULL,
         signalOut = NULL,
         prevResults = NULL,
         path
      )
      {
         if(!is.null(signalIn)) {
            self$signalIn <- signalIn;
         } else {
            if(is.null(self$signalIn)) {
               stop(paste(
                  "Input signal not provided to SignalDerivation$derive."
               ));
            }
         }
         if(!is.null(signalOut)) {
            self$signalOut <- signalOut;
         } else {
            if(is.null(self$signalOut)) {
               stop(paste(
                  "Output signal not provided to SignalDerivation$derive."
               ));
            }
         }

         if (!is.null(self$staticAirPressure)) {
            airPressure <-
               rep(x = 0, times = self$signalOut$getLength()) +
               self$staticAirPressure;
         } else {
            airPressure <- self$signalOut$getVariable(self$airPressureHeader);
         }

         upstreamDO <- self$signalIn$getVariable(self$doHeader);
         upstreamTemp <- self$signalIn$getVariable(self$tempHeader);
         downstreamTemp <- self$signalOut$getVariable(self$tempHeader);
         upstreamPAR <- self$signalIn$getVariable(self$parHeader);
         downstreamPAR <- self$signalOut$getVariable(self$parHeader);

         validIndices <-
            is.finite(upstreamDO) &
            is.finite(upstreamTemp) &
            is.finite(downstreamTemp) &
            is.finite(upstreamPAR) &
            is.finite(downstreamPAR) &
            is.finite(airPressure);

         if(!self$usepCO2) {
            model <- CMetabLagrangeDo$new(
               type = self$modelType,
               dailyGPP = self$initParams["dailyGPP"],
               dailyER = self$initParams["dailyER"],
               k600 = self$initParams["k600"],
               upstreamDO = upstreamDO[validIndices],
               upstreamTime = self$signalIn$getTime()[validIndices],
               downstreamTime = self$signalOut$getTime()[validIndices],
               upstreamTemp = upstreamTemp[validIndices],
               downstreamTemp = downstreamTemp[validIndices],
               upstreamPAR = upstreamPAR[validIndices],
               downstreamPAR = downstreamPAR[validIndices],
               airPressure = airPressure[validIndices],
               stdAirPressure = 1
            );
         } else {
            upstreamDIC <- self$signalIn$getVariable(self$dicHeader);

            if (!is.null(self$staticCO2Air)) {
               co2Air <-
                  rep(x = 0, times = self$signalOut$getLength()) +
                  self$staticCO2Air;
            } else {
               co2Air <- self$signalOut$getVariable(self$co2AirHeader);
            }

            if (!is.null(self$staticAlkalinity)) {
               upstreamAlkalinity <-
                  rep(x = 0, times = self$signalOut$getLength()) +
                  self$staticAlkalinity;
               downstreamAlkalinity <-
                  rep(x = 0, times = self$signalOut$getLength()) +
                  self$staticAlkalinity;
            } else {
               upstreamAlkalinity <-
                  self$signalIn$getVariable(self$alkalinityHeader);
               downstreamAlkalinity <-
                  self$signalOut$getVariable(self$alkalinityHeader);
            }

            validIndices <-
               validIndices &
               is.finite(upstreamDIC) &
               is.finite(co2Air) &
               is.finite(upstreamAlkalinity) &
               is.finite(downstreamAlkalinity);

            model <- CMetabLagrangeDoDic$new(
               type = self$modelType,
               dailyGPP = self$initParams["dailyGPP"],
               dailyER = self$initParams["dailyER"],
               k600 = self$initParams["k600"],
               upstreamDO = upstreamDO[validIndices],
               upstreamTime = self$signalIn$getTime()[validIndices],
               downstreamTime = self$signalOut$getTime()[validIndices],
               upstreamTemp = upstreamTemp[validIndices],
               downstreamTemp = downstreamTemp[validIndices],
               upstreamPAR = upstreamPAR[validIndices],
               downstreamPAR = downstreamPAR[validIndices],
               airPressure = airPressure[validIndices],
               stdAirPressure = 1,
               upstreamDIC = upstreamDIC[validIndices],
               pCO2air = co2Air[validIndices],
               upstreamAlkalinity = upstreamAlkalinity[validIndices],
               downstreamAlkalinity = downstreamAlkalinity[validIndices]
            );
         }

         self$objFunc$setModel(model = model);

         if (self$useDO) {
            observation <- data.frame(do = self$signalOut$getVariable(self$doHeader));
            if (self$usepCO2) {
               observation$pCO2 <- self$signalOut$getVariable(self$pCO2Header);
            }
         } else if (self$usepCO2) {
            observation <- data.frame(pCO2 = self$signalOut$getVariable(self$pCO2Header));
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
            upstreamTime = self$signalIn$getTime()[validIndices],
            downstreamTime = self$signalIn$getTime()[validIndices],
            upstreamTemp = upstreamTemp[validIndices],
            downstreamTemp = downstreamTemp[validIndices],
            upstreamPAR = upstreamPAR[validIndices],
            downstreamPAR = downstreamPAR[validIndices],
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
