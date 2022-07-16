rmarkdown::render(
  input = "./02_protocol/01_test_basic.Rmd",
  output_format = "html_document",
  output_file = "test_basic.html",
  output_dir = "./04_product",
  knit_root_dir = getwd()
)