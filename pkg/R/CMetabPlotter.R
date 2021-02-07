# Package dependencies ####

# R oxygen code for importing the proper classes used in this file
# Used for robust namespace management in R packages
#' @importFrom R6 R6Class
NULL

# Class OneStationMetabPlotter (R6) ####

#' @export
#'
#' @title
#'   R6 class defining a plotter for a one station metabolism DO model
#'
#' @description
#'   Plots a signal of DO and pCO2 from a one station model of metabolism
#'
CMetabPlotter <- R6Class(
   classname = "CMetabPlotter",
   inherit = disco::SignalPlotter,
   public = list(

      #' @field timeTicks
      #'   Number of ticks on the time axis
      timeTicks = NULL,

      #' @field resultFile
      #'   Base file name for the results.
      resultFile = NULL,

      #' @field results
      #'   The results object to be plotted
      results = NULL,

      #' @field plotDIC
      #'   Flag to determine if DIC should be plotted rather
      #'   than pCO2.
      plotDIC = NULL,

      #' @field subTitle
      #'   Subtitle for headings on plots, will appear over right
      #'   panel on legend.
      subTitle = NULL,

      #' @field format
      #'   Data frame aggregating the formats for all the plots
      format = NULL,

      #' @field doHeader
      #' Character string reperesenting the header for dissolved oxygen data
      doHeader = NULL,

      #' @field pCO2Header
      #' Character string reperesenting the header for dissolved carbon dioxide data.
      pCO2Header = NULL,

      #' @field dicHeader
      #' Character string reperesenting the header for dissolved inorganic carbon data.
      dicHeader = NULL,

      #' @field parHeader
      #' Character string reperesenting the header for PAR data.
      parHeader = NULL,

      #' @field tempHeader
      #' Character string reperesenting the header for temperature data.
      tempHeader = NULL,

      # Method OneStationMetabPlotter$new ####
      #
      #' @description
      #'   Construct a new instance of the class.
      #'
      #' @param ...
      #'   Arguments passed to the constructor of the super class
      #' @param timeTicks
      #'   Number of ticks on the time axis
      #' @param resultFile
      #'   Base file name for the results.
      #'   Defaults to "results".
      #' @param plotDIC
      #'   Flag to determine if DIC should be plotted rather
      #'   than pCO2.
      #'   Defaults to FALSE.
      #' @param subTitle
      #'   Subtitle for headings on plots, will appear over right
      #'   panel on legend.
      #'   Defaults to an empty string (blank).
      #' @param doHeader
      #'   Optional character string reperesenting the header for dissolved oxygen data
      #'   Defaults to "do".
      #' @param dicHeader
      #'   Optional character string reperesenting the header for dissolved inorganic carbon data.
      #'   Defaults to "dic".
      #' @param pCO2Header
      #'   Optional character string reperesenting the header for dissolved carbon dioxide data.
      #'   Defaults to "pCO2".
      #' @param parHeader
      #'   Optional character string reperesenting the header for PAR data.
      #'   Defaults to "par".
      #' @param tempHeader
      #'   Optional character string reperesenting the header for PAR data.
      #'   Defaults to "temp"
      #' @param format.doObs
      #'   Format for DO observations.
      #'   See defaults in usage.
      #' @param format.pCO2Obs
      #'   Format for pCO2 observations.
      #'   See defaults in usage.
      #' @param format.dicObs
      #'   Format for DIC observations.
      #'   See defaults in usage.
      #' @param format.doPred
      #'   Format for DO predictions
      #'   See defaults in usage.
      #' @param format.pCO2Pred
      #'   Format for pCO2 predictions
      #'   See defaults in usage.
      #' @param format.dicPred
      #'   Format for DIC predictions
      #'   See defaults in usage.
      #' @param format.doSat
      #'   Format for saturation DO concentrations
      #'   See defaults in usage.
      #' @param format.par
      #'   Format for PAR
      #'   See defaults in usage.
      #' @param format.temp
      #'   Format for temperature
      #'   See defaults in usage.
      #'
      initialize = function
      (
         ...,
         timeTicks,
         resultFile = "results",
         plotDIC = FALSE,
         subTitle = "",
         doHeader = "do",
         pCO2Header = "pCO2",
         dicHeader = "dic",
         parHeader = "par",
         tempHeader = "temp",
         format.doObs = list(
            name = "[DO] observed",
            lty = NA,
            pch = 1,
            col = "black",
            lwd = 1
         ),
         format.pCO2Obs = list(
            name = "pCO2 or DIC observed",
            lty = NA,
            pch = 1,
            col = "green3",
            lwd = 1
         ),
         format.dicObs = list(
            name = "DIC Obs",
            lty = NA,
            pch = 1,
            col = "green3",
            lwd = 1
         ),
         format.doPred = list(
            name = "[DO] modeled",
            lty = "solid",
            pch = NA,
            col = "black",
            lwd = 2
         ),
         format.pCO2Pred = list(
            name = "pCO2 or DIC modeled",
            lty = "solid",
            pch = NA,
            col = "green3",
            lwd = 2
         ),
         format.dicPred = list(
            name = "DIC Pred",
            lty = "solid",
            pch = NA,
            col = "green3",
            lwd = 2
         ),
         format.doSat = list(
            name = "Saturated [DO]",
            lty = "dotted",
            pch = NA,
            col = "black",
            lwd = 2
         ),
         format.par = list(
            name = "PAR",
            lty = "dashed",
            pch = NA,
            col = "red",
            lwd = 2
         ),
         format.temp = list(
            name = "Temperature",
            lty = NA,
            pch = 1,
            col = "black",
            lwd = 1
         )
      )
      {
         # Call the constructor of the super class

         super$initialize(...);

         # Populate attributes

         self$timeTicks <- timeTicks;
         self$resultFile <- resultFile;
         self$plotDIC <- plotDIC;
         self$subTitle <- subTitle;
         self$doHeader <- doHeader;
         self$pCO2Header <- pCO2Header;
         self$dicHeader <- dicHeader;
         self$parHeader <- parHeader;
         self$tempHeader <- tempHeader;

         # Set the format scheme for plots
         self$format <- rbind.data.frame(
            doObs = format.doObs,
            pCO2Obs = format.pCO2Obs,
            dicObs = format.dicObs,
            doPred = format.doPred,
            pCO2Pred = format.pCO2Pred,
            dicPred = format.dicPred,
            doSat = format.doSat,
            par = format.par,
            temp = format.temp,
            stringsAsFactors = FALSE
         );
      },

      # Method OneStationMetabPlotter$open ####
      #
      #' @description
      #'   Opens the pdf for writing the plots
      #'
      #' @param path
      #'   The path where the pdf of figures should be written
      #'
      #' @return
      #'   No defined return value
      #'
      open = function
      (
         path
      )
      {
         super$open(path);

         plot.new();
         plot.window(xlim = 0:1, ylim = 0:1);
         title(main = "One Station Metabolism Analysis Summary");

         text(
            x = 0.5,
            y = 0.9,
            labels = "Left panel legend:",
            font = 2
         );
         ordered <- self$format[
            c("doObs", "doPred", "pCO2Obs", "pCO2Pred", "doSat", "par"),
            # All columns
         ];
         legend(
            x = "center",
            bty = 'n',
            legend = ordered$name,
            lty = ordered$lty,
            lwd = ordered$lwd,
            pch = ordered$pch,
            col = ordered$col
         );

         plot.new();
         plot.window(xlim = 0:1, ylim = 0:1);
         title(main = self$subTitle)

         text(
            x = 0.5,
            y = 0.7,
            labels = "Right panel legend:",
            font = 2
         );
         ordered <- self$format[
            c("temp", "par"),
            # All columns
         ];
         legend(
            x = "center",
            bty = 'n',
            legend = ordered$name,
            lty = ordered$lty,
            lwd = ordered$lwd,
            pch = ordered$pch,
            col = ordered$col
         );
      },

      # Method OneStationMetabPlotter$summarize ####
      #
      #' @description
      #'   Summarize the data in a two panel visualization
      #'
      #' @param signal
      #'   The signal object that contains observations
      #' @param outputPath
      #'   A path to results of a metabolism analysis.
      #'   Not necessary if summarizer does not need output.
      #' @param label
      #'   A label for the summary
      #' @param timeBounds
      #'   The temporal bounds on the summary
      #'
      #' @return
      #'   No defined return value
      #'
      summarize = function
      (
         signal = NULL,
         outputPath = NULL,
         label,
         timeBounds
      )
      {
         # Set the signal if it is provided
         if (!is.null(signal)) {
            self$signal <- signal;
         }
         # The signal is not plotted if the signal is not provided
         plotSignal <- !is.null(self$signal);

         # Set the output path if provided
         if(!is.null(outputPath)) {
            self$outputPath <- outputPath;
         }
         # Results are not plotted if output path is not provided
         plotResults <- !is.null(self$outputPath);

         # Load the results file if model results are to be plotted
         if(plotResults) {
            self$results <- readRDS(file = sprintf(
               fmt = "%s/%s.RData",
               self$outputPath,
               self$resultFile
            ));
         }

         if(!(plotSignal || plotResults)) {
            stop("Plotter needs at least one of signal or model results to plot.")
         }

         # Plot DO

         # Plot the DO observations
         if(plotSignal) {
            ylim <- c(
               min(self$signal$getVariable(self$doHeader), na.rm = TRUE),
               max(self$signal$getVariable(self$doHeader), na.rm = TRUE)
            );
            if(plotResults) {
               y.res.vars <- list(
                  do = self$results$pred$do$dox,
                  doSat = self$results$pred$do$doSat
               );
               allvals <- c(ylim, unlist(y.res.vars));
               ylim <- c(
                  min(allvals, na.rm = TRUE),
                  max(allvals, na.rm = TRUE)
               );
            }

            format <- self$format["doObs",];
            self$signal$plot(
               header = self$doHeader,
               xaxt = "n",
               ylim = ylim,
               ylab = "",
               pch = format$pch,
               col = format$col,
               lwd = format$lwd
            );

            if(plotResults) {
               format <- self$format["doSat",];
               lines(
                  x = self$results$time,
                  y = y.res.vars$doSat,
                  lty = format$lty,
                  col = format$col,
                  lwd = format$lwd
               );
               format <- self$format["doPred",];
               lines(
                  x = self$results$time,
                  y = y.res.vars$do,
                  lty = format$lty,
                  col = format$col,
                  lwd = format$lwd
               );
               mtext(
                  text = bquote(paste(
                     .(sprintf(
                        fmt = "GPP = %1.2e",
                        self$results$params["dailyGPP"]
                     )),
                     ~mu*mol~L^-1~d^-1
                  )),
                  side = 3,
                  line = 0.5,
                  cex = 0.6,
                  adj = 1
               );
            }
         } else {
            y.res.vars <- list(
               do = self$results$pred$do$dox,
               doSat = self$results$pred$doSat
            );
            allvals <- unlist(y.res.vars);
            ylim <- c(
               min(allvals, na.rm = TRUE),
               max(allvals, na.rm = TRUE)
            );
            format <- self$format["doSat",];
            plot(
               x = self$results$time,
               y = y.res.vars$doSat,
               type = "l",
               xaxt = "n",
               xlab = "time",
               ylim = ylim,
               ylab = "",
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
            format <- self$format["doPred",];
            lines(
               x = self$results$time,
               y = y.res.vars$do,
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
            mtext(
               text = bquote(paste(
                  .(sprintf(
                     fmt = "GPP = %1.2e",
                     self$results$params["dailyGPP"]
                  )),
                  ~mu*mol~L^-1~d^-1
               )),
               side = 3,
               line = 0.5,
               cex = 0.6,
               adj = 1
            );
         }
         mtext(
            text = bquote(.("[DO] (")*mu*mol~L^-1*.(")")),
            side = 2,
            line = 2,
            cex = 0.7,
            col = self$format["doObs", "col"]
         );
         axis.POSIXct(
            side = 1,
            at = seq(
               timeBounds[1],
               timeBounds[2],
               length.out = self$timeTicks
            ),
            format = "%H:%M"
         );
         mtext(
            side = 3,
            line = 0.5,
            adj = 0,
            text = label,
            cex = 0.7
         );

         # Reset the axes scaling for inorganic carbon
         par(new = TRUE);

         if (plotSignal) {
            plotDICSignal <- !is.null(self$signal$getVariable(self$pCO2Header));
         } else {
            plotDICSignal <- FALSE;
         }
         if (plotResults) {
            plotDICResults <- !is.null(self$results$pred$dic$pCO2);
         } else {
            plotDICResults <- FALSE;
         }

         if (!self$plotDIC) {

            # Plot the pCO2 observations
            if (plotSignal && plotDICSignal) {
               ylim <- c(
                  min(self$signal$getVariable(self$pCO2Header), na.rm = TRUE),
                  max(self$signal$getVariable(self$pCO2Header), na.rm = TRUE)
               );
               if(plotResults && plotDICResults) {
                  allvals <- c(ylim, self$results$pred$dic$pCO2);
                  ylim <- c(
                     min(allvals, na.rm = TRUE),
                     max(allvals, na.rm = TRUE)
                  );
               }
               format <- self$format["pCO2Obs",];
               self$signal$plot(
                  header = self$pCO2Header,
                  xaxt = "n",
                  yaxt = "n",
                  ylim = ylim,
                  ylab = "",
                  pch = format$pch,
                  col = format$col,
                  lwd = format$lwd
               );
               if (plotResults && plotDICResults) {
                  if (!is.null(self$results$pred$dic$pCO2)) {
                     format <- self$format["pCO2Pred",];
                     lines(
                        x = self$results$time,
                        y = self$results$pred$dic$pCO2,
                        lty = format$lty,
                        col = format$col,
                        lwd = format$lwd
                     );
                  }
               }
            } else if (plotDICResults) {
               ylim <- c(
                  min(self$results$pred$dic$pCO2, na.rm = TRUE),
                  max(self$results$pred$dic$pCO2, na.rm = TRUE)
               );
               if (!is.null(self$results$pred$dic$pCO2)) {
                  format <- self$format["pCO2Pred",];
                  plot(
                     x = self$results$time,
                     y = self$results$pred$dic$pCO2,
                     type = "l",
                     xaxt = "n",
                     xlab = "",
                     yaxt = "n",
                     ylim = ylim,
                     ylab = "",
                     lty = format$lty,
                     col = format$col,
                     lwd = format$lwd
                  );
               }
            }
            if(plotDICSignal || plotDICResults) {
               axis(
                  side = 4
               );
               mtext(
                  text = bquote(pCO[2]~.("(")*mu*atm*.(")")),
                  side = 4,
                  line = 2.5,
                  cex = 0.7,
                  col = self$format["pCO2Obs", "col"]
               );
            }

         } else {

            # Plot the DIC observations
            if(plotSignal && plotDICSignal) {
               ylim <- c(
                  min(self$signal$getVariable(self$dicHeader), na.rm = TRUE),
                  max(self$signal$getVariable(self$dicHeader), na.rm = TRUE)
               );
               if(plotResults && plotDICResults) {
                  allvals <- c(ylim, self$results$pred$dic$dic);
                  ylim <- c(
                     min(allvals, na.rm = TRUE),
                     max(allvals, na.rm = TRUE)
                  );
               }
               format <- self$format["dicObs",];
               self$signal$plot(
                  header = self$dicHeader,
                  xaxt = "n",
                  xlab = "",
                  ylim = ylim,
                  yaxt = "n",
                  ylab = "",
                  pch = format$pch,
                  col = format$col,
                  lwd = format$lwd
               );
               if(plotResults && plotDICResults) {
                  format <- self$format["dicPred",];
                  lines(
                     x = self$results$time,
                     y = self$results$pred$dic$dic,
                     lty = format$lty,
                     col = format$col,
                     lwd = format$lwd
                  );
               }
            } else if (plotDICResults) {
               ylim <- c(
                  min(self$results$pred$dic$dic, na.rm = TRUE),
                  max(self$results$pred$dic$dic, na.rm = TRUE)
               );
               format <- self$format["dicPred",];
               plot(
                  x = self$results$time,
                  y = self$results$pred$dic$dic,
                  type = "l",
                  xaxt = "n",
                  xlab = "",
                  ylim = ylim,
                  yaxt = "n",
                  ylab = "",
                  lty = format$lty,
                  col = format$col,
                  lwd = format$lwd
               );
            }
            if(plotDICSignal || plotDICResults) {
               axis(
                  side = 4
               );
               mtext(
                  text = bquote(.("[DIC] (")*mu*mol~L^-1*.(")")),
                  side = 4,
                  line = 2.5,
                  cex = 0.7,
                  col = self$format["dicObs", "col"]
               );
            }

         }

         # Reset the axes scaling for PAR reference
         par(new = TRUE);

         format <- self$format["par",];
         if(plotSignal) {
            self$signal$plot(
               header = self$parHeader,
               type = "l",
               yaxt = "n",
               ylab = "",
               xaxt = "n",
               xlab = "",
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
         } else {
            plot(
               x = self$results$time,
               y = self$results$par,
               type = "l",
               yaxt = "n",
               ylab = "",
               xaxt = "n",
               xlab = "",
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
         }

         # Plot temperature on a new plot
         format <- self$format["temp",];
         if(plotSignal) {
            self$signal$plot(
               header = self$tempHeader,
               xaxt = "n",
               pch = format$pch,
               col = format$col,
               lwd = format$lwd
            );
         } else {
            plot(
               x = self$results$time,
               y = self$results$temp,
               ylab = "temp (deg C)",
               xaxt = "n",
               xlab = "time",
               pch = format$pch,
               col = format$col,
               lwd = format$lwd
            );
         }
         axis.POSIXct(
            side = 1,
            at = seq(
               timeBounds[1],
               timeBounds[2],
               length.out = self$timeTicks
            ),
            format = "%H:%M"
         );
         if (plotResults) {
            mtext(
               text = bquote(paste(
                  .(sprintf(
                     fmt = "ER = %1.2e",
                     self$results$params["dailyER"]
                  )),
                  ~mu*mol~L^-1~d^-1
               )),
               side = 3,
               line = 0.5,
               cex = 0.6,
               adj = 0
            );
            mtext(
               text = bquote(paste(
                  k[600],
                  .(sprintf(
                     fmt = " = %1.2e",
                     self$results$params["k600"]
                  )),
                  ~d^-1
               )),
               side = 3,
               line = 0.5,
               cex = 0.6,
               adj = 1
            );
         }

         # Reset axes scaling for PAR
         par(new = TRUE);

         # Plot PAR on the temperature plot
         format <- self$format["par",];
         if(plotSignal) {
            self$signal$plot(
               header = self$parHeader,
               type = "l",
               yaxt = "n",
               ylab = "",
               xaxt = "n",
               xlab = "",
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
         } else {
            plot(
               x = self$results$time,
               y = self$results$par,
               type = "l",
               yaxt = "n",
               ylab = "",
               xaxt = "n",
               xlab = "",
               lty = format$lty,
               col = format$col,
               lwd = format$lwd
            );
         }
         axis(
            side = 4
         );
         mtext(
            text = "PAR (relative units)",
            side = 4,
            line = 2.5,
            cex = 0.7,
            col = self$format["par", "col"]
         );
      }
   )
)
