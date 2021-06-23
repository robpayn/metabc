# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

# Class ParameterTranslatorMetabc (R6) ####

#' @export
#'
#' @title
#'    R6 class defining a parameter translator for metabolism models
#'
#' @description
#'    A parameter translator allowing GPP, ER, and k600 to be used
#'    as proposed parameters for an objective function using a
#'    stream metabolism model.
#'
ParameterTranslatorMetabc <- R6Class(
   classname = "ParameterTranslatorMetabc",
   inherit = inferno::ParameterTranslator,
   public = list(

      # Method ParameterTranslatorMetab$translate ####
      #
      #' @description
      #'   Used to translate 3 parameters in the order of
      #'   gross primary production, ecosystem respiration,
      #'   and gas exchange at a Schmidt number of 600
      #'
      #' @param params
      #'   Named vector of values to use for GPP (named "dailyGPP"),
      #'   ER (named "dailyER"), and k600 (name "k600") parameters.
      #'   Parameters with missing names will not be translated.
      #'   Any additional elements will be ignored.
      #'
      #' @return
      #'   No defined return value.
      #'
      translate = function
      (
         params
      )
      {
         gppValue <- params["dailyGPP"];
         erValue <- params["dailyER"];
         k600Value <- params["k600"];
         if (!is.na(gppValue)) {
            self$model$setMetabParam(param = "DailyGPP", value = gppValue);
         }
         if (!is.na(erValue)) {
            self$model$setMetabParam(param = "DailyER", value = erValue);
         }
         if (!is.na(k600Value)) {
            self$model$setMetabParam(param = "k600", value = k600Value);
         }
         return( NULL );
      }
   )
)
