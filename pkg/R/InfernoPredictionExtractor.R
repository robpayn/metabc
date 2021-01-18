# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

# Class PredictionExtractorMetabc (R6) ####

#' @export
#'
#' @title
#'    R6 class defining a prediction extractor for DO from metabolism models
#'
#' @description
#'    A prediction extractor allowing dissolved oxygen outputs
#'    from a stream metabolism model to be used as predictions
#'    in an objective function.
#'
PredictionExtractorMetabc <- R6Class(
   classname = "PredictionExtractorMetabc",
   inherit = inferno::PredictionExtractor,
   public = list(

      #' @field extractDO
      #'   Logical value indicating if DO predictions should be extracted
      extractDO = NULL,

      #' @field extractCO2
      #'   Logical value indicating if pCO2 predictions should be extracted
      extractCO2 = NULL,

      #' @description
      #'   Create a new instance of the class with provided values for
      #'   extraction flags
      #'
      #' @param ...
      #'   Arguments passed to PredictionExtractor super class
      #' @param extractDO
      #'   Logical value indicating if DO predictions should be extracted
      #' @param extractCO2
      #'   Logical value indicating if pCO2 predictions should be extracted

      #' @return
      #'   A new instance of the class
      #'
      initialize = function(..., extractDO, extractCO2)
      {
         # Run the constructor for PredictionExtractor super class
         super$initialize(...);

         # Set attributes
         self$extractDO <- extractDO;
         self$extractCO2 <- extractCO2;

         if (!(extractDO || extractCO2)) {
           stop("Prediction extractor is not configured to extract any variables.");
         };
      },

      # Method PredictionExtractorMetabDo$extract ####
      #
      #' @description
      #'   Used to extract dissolved oxygen predictions from a metabolism model
      #'
      #' @return
      #'   A data frame with a single column of DO data
      #'
      extract = function()
      {
         if (self$extractDO) {
            df <- data.frame(do = self$model$output$do$dox);
            if (self$extractCO2) {
               df$pCO2 <- self$model$output$dic$pCO2;
            }
         } else if (self$extractCO2) {
            df <- data.frame(pCO2 = self$model$output$dic$pCO2);
         } else {
            stop("Prediction extractor is not configured to extract any variables.");
         }
         return( df );
      }
   )
)
