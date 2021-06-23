rmarkdown::render(
   input = "./02_protocol/gw/test.Rmd",
   output_format = "html_document",
   output_file = "GroundwaterTestResults.html",
   output_dir = "./04_product",
   knit_root_dir = getwd()
)