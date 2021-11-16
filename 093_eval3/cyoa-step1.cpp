#include <iostream>

#include "page.hpp"

int main(int argc, char ** argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./cyoa-step1 pagexx.txt" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string fileName(argv[1]);
  Page page(fileName);
  page.printPage();
  return EXIT_SUCCESS;
}
