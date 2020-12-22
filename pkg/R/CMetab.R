# Dependencies for ROxygen ####

#' @importFrom R6 R6Class
#' @useDynLib metabc

# Class CMatab ####

#' @export
#'
#' @title
#'   Time series DO metabolism model based on C++ implementation
#'
#' @description
#'   Abstract class defining the base functionality of a metabolism
#'   model in aquatic systems.
#'
#'   Implementation of models derived from this class is provided by C++ objects.
#'
CMetab <- R6Class(
   classname = "CMetab",
   public = list(

      # Class CMetab: Attributes ####

      #' @field pointers
      #'   A list of external pointers to the C++ objects associated with
      #'   implementing the features of the model.
      pointers = NULL,

      # Call CMetab: Methods ####

      #' @description
      #'   Runs a simulation based on parameters and driving data
      #'   in the object attributes. Output will be stored in the
      #'   output attribute, but is also returned from this function
      #'   as a convenience.
      #'
      #' @return
      #'   The SEXP object returned by the call to the C++ method
      #'
      run = function()
      {
         .Call(
            "Metab_run",
            self$pointers$metabExternalPointer
         );
      },

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
