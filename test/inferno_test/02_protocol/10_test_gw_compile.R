rmarkdown::render(
  input = "./02_protocol/10_test_gw.Rmd",
  output_format = "html_document",
  output_file = "test_gw.html",
  output_dir = "./04_product",
  knit_root_dir = getwd()
)