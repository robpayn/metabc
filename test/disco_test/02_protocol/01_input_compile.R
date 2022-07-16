rmarkdown::render(
  input = "./02_protocol/01_input.Rmd",
  output_format = "html_document",
  output_file = "notes_01_input.html",
  output_dir = "./04_product",
  knit_root_dir = getwd()
)