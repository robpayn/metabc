# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @useDynLib metabc

#' @export
#'
#' @title
#'   Carbonate equilibrium calculator
#'
#' @description
#'   Provides tools for calculating activities of inorganic carbon
#'   species based on equilibrium constants.
#'
#'   Implementation is provided by C++ objects.
#'
CCarbonateEq <- R6Class(
   classname = "CCarbonateEq",
   public = list(

      #' @field externalPointer
      #'   The external pointer to the associated C++ CarbonateEq object
      externalPointer = NULL,

      #' @description
      #'   Constructs an object that is a new instance of the class
      #'
      #' @param tempC
      #'    A numerical value for the temperature
      #' @param eConduct
      #'    An optonal numerical value for the electrical conductivity
      #'    Default value is 0 (results in an ionic strength of 0)
      #'
      initialize = function(tempC, eConduct = 0)
      {
         self$externalPointer = .Call("CarbonateEq_constructor", tempC, eConduct)
      },

      #' @description
      #'   Resets the calculator to a new temperature and electrical conductivity
      #'
      #' @param tempC
      #'    A numerical value for the temperature
      #' @param eConduct
      #'    An optonal numerical value for the electrical conductivity
      #'    Default value is 0 (results in an ionic strength of 0)
      #'
      reset = function(tempC, eConduct = 0)
      {
         .Call(
            "CarbonateEq_reset",
            self$externalPointer,
            tempC,
            eConduct
         )
         invisible(NULL);
      },

      #' @description
      #'   Performs an optimization calculation to estimate the partial
      #'   pressure of carbon dioxide based on a known DIC and total
      #'   alkalinity.
      #'
      #' @param dic
      #'   Concentration of DIC.
      #'   Units of molarity.
      #' @param totalAlk
      #'   Concentration of total alkalinity.
      #'   Units of molarity.
      #' @param pHTol
      #'   Optional value for tolerance of pH optimization.
      #'   Default value is 1e-5.
      #' @param pHMin
      #'   Optional value for the minimum pH value for optimization.
      #'   Default value is 2.
      #' @param pHMax
      #'   Optional value for the maximum pH value for optimization.
      #'   Default value is 12.
      #'
      optfCO2FromDICTotalAlk = function(
         dic,
         totalAlk,
         pHTol = 1e-5,
         pHMin = 2,
         pHMax = 12
      )
      {
         return(
            .Call(
               "CarbonateEq_optfCO2FromDICTotalAlk",
               self$externalPointer,
               dic,
               totalAlk,
               pHTol,
               pHMin,
               pHMax
            )
         )
      }
   )
)
