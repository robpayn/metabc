# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @useDynLib metabc

# R6 Class CMetab ####

#' @export
#'
#' @title
#'   C++ model of aqueous metabolism influence on oxygen
#'
#' @description
#'   Abstract class (should not be instantiated on its own) defining
#'   the base functionality of a metabolism model in aquatic systems.
#'
#'   Implementations of models derived from this class are provided
#'   by C++ objects. See \link{CMetabDo} as an example of a fully
#'   functional implementation of this abstraction.
#'
CMetab <- R6Class(
   classname = "CMetab",
   public = list(

      ## Attributes ####

      #' @field pointers
      #'   A list of external pointers to the C++ objects associated with
      #'   implementing the features of the model.
      pointers = NULL,

      ## Method: run ####

      #' @description
      #'   Runs a simulation based on parameters and driving data
      #'   in the object attributes. Output will be stored in the
      #'   output attribute, but is also returned from this function
      #'   as a convenience.
      #'
      #' @return
      #'   The S expression object returned by the call to the C++ method
      #'
      run = function()
      {
         .Call(
            "Metab_run",
            self$pointers$metabExternalPointer
         );
      },

      ## Method: setMetabParam ####

      #' @description
      #'   Sets one of the model parameters to a new value.
      #'
      #' @param param
      #'   A character string to indicate the parameter to be changed
      #'   \itemize{
      #'     \item "DailyGPP": change the daily GPP parameter
      #'     \item "DailyER": change the daily ER parameter
      #'     \item "k600": change the k600 parameter
      #'   }
      #' @param value
      #'   The new value for the parameter
      #'
      #' @return
      #'   The previous value of the parameter
      #'
      setMetabParam = function(param, value)
      {
         .Call(
            sprintf("Metab_set%s", param),
            self$pointers$metabExternalPointer,
            value
         )
      }
   )
)
