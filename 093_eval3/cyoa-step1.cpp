#include <iostream>

#include "page.hpp"

int main(int argc, char ** argv) {
  /* Command Check. */
  if (argc != 2) {
    std::cerr << "Usage: ./cyoa-step1 pagexx.txt" << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Extract fileName to string. */
  std::string fileName(argv[1]);
  /* Instantiate page object. */
  Page page(fileName);
  page.printPage();
  return EXIT_SUCCESS;
}
