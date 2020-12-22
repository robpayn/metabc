rmarkdown::render(
   input = "./02_protocol/R6/test.Rmd",
   output_format = "html_document",
   output_file = "R6LevelTestResults.html",
   output_dir = "./04_product",
   knit_root_dir = getwd()
)