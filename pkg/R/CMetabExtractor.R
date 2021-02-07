# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

#' @export
#'
#' @title
#'   R6 class for extracting results from one station metabolism models
#'
#' @description
#'   Extracts parameters used from metabolism models
#'
CMetabExtractor <- R6Class(
   classname = "CMetabExtractor",
   inherit = disco::SignalSummarizer,
   public = list(

      #' @field table
      #'   Table of summary data
      table = NULL,

      #' @field index
      #'   Index for tracking the row in the table
      index = NULL,

      #' @field resultFile
      #'   The name of the file with analysis results
      resultFile = NULL,

      #' @field summaryFile
      #'   The name of the file to create with the summary
      summaryFile = NULL,

      #' @field path
      #'   Table of summary data
      path = NULL,

      # Method OneStationMetabExtractor$new ####
      #
      #' @description
      #'   Constructs a new instance of the class
      #'
      #' @param rows
      #'   number of rows in the table of extracted parameter values
      #' @param resultFile
      #'   name of the results files to read
      #' @param summaryFile
      #'   name of the summary file to create
      #'
      initialize = function
      (
         rows,
         resultFile = "results",
         summaryFile = "summary"
      )
      {
         column <- rep(as.numeric(NA), times = rows);
         self$table <- data.frame(
            time = rep(as.POSIXct("1970-01-01 00:00:00"), time = rows),
            dailyGPP = column,
            dailyER = column,
            k600 = column
         );
         self$index <- 1;
         self$resultFile <- resultFile;
         self$summaryFile <- summaryFile;
      },

      # Method OneStationMetabExtractor$open ####
      #
      #' @description
      #'   Prepare to write the summary file
      #'
      #' @param path
      #'   Path where the summary file will be written
      #'
      #' @return
      #'   No defined return value.
      #'
      open = function(path)
      {
         self$path = path;
      },

      # Method OneStationMetabExtractor$summarize ####
      #
      #' @description
      #'   Collects the data for the summary
      #'
      #' @param signal
      #'   The signal being summarized
      #' @param outputPath
      #'   The path where analysis output is located
      #' @param label
      #'   A label for the summary
      #' @param timeBounds
      #'   The temporal bounds on the summary
      #'
      #' @return
      #'   No defined return value.
      #'
      summarize = function
      (
         signal,
         outputPath,
         label,
         timeBounds
      )
      {
         results <- readRDS(file = sprintf(
            fmt = "%s/%s.RData",
            outputPath,
            self$resultFile
         ));
         attributes(self$table$time)$tzone <-
            attributes(results$time)$tzone;
         self$table$time[self$index] <- results$time[1];
         self$table$dailyGPP[self$index] <- results$params["dailyGPP"];
         self$table$dailyER[self$index] <- results$params["dailyER"];
         self$table$k600[self$index] <- results$params["k600"];
         self$index <- self$index + 1;
      },

      # Method OneStationMetabExtractor$close ####
      #
      #' @description
      #'   Closes the summary output
      #'
      #' @return
      #'   No defined return value.
      #'
      close = function()
      {
         signal <- disco::Signal$new(
            self$table,
            metaColumns = data.frame(
               time = c(
                  sprintf("Gregorian %s", attributes(self$table$time)$tzone),
                  "Date and time",
                  "Locality"
               ),
               dailyGPP = c(
                  "micromolarity per day",
                  "gross primary production",
                  "aquatic ecosystem"
               ),
               dailyER = c(
                  "micromolarity per day",
                  "ecosystem respiration",
                  "aquatic ecosystem"
               ),
               k600 = c(
                  "per day",
                  "atmospheric gas exchange rate constant",
                  "aquatic ecosystem"
               ),
               row.names = c(
                  "units",
                  "property",
                  "entity"
               )
            )
         );

         saveRDS(
            signal,
            file = sprintf(
               fmt = "%s/%s.RData",
               self$path,
               self$summaryFile
            )
         );

         signal$writeCSV(
            path = self$path,
            name = self$summaryFile
         );
      }

   )
)
