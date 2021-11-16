#include "story.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "page.hpp"

void generateFileName(std::stringstream & filePath,
                      const std::string & directory,
                      size_t pageNumber);
bool isFileExist(const std::string filePath);

Story::Story(const std::string & directory) {
  std::stringstream filePath;
  size_t pageNumber = 1;
  generateFileName(filePath, directory, pageNumber);
  while (isFileExist(filePath.str())) {
    this->addPage(Page(filePath.str()));
    pageNumber++;
    generateFileName(filePath, directory, pageNumber);
  }
  if (pageNumber == 1) {
    std::cerr << "page1.txt does not exist. " << std::endl;
    exit(EXIT_FAILURE);
  }
}

void generateFileName(std::stringstream & filePath,
                      const std::string & directory,
                      size_t pageNumber) {
  /* Clear the stingstream and reset its status for new file path generation*/
  filePath.str(std::string());
  filePath.clear();
  filePath << directory << "/page" << pageNumber << ".txt";
}

bool isFileExist(const std::string & filePath) {
  std::ifstream file(filePath);
  return file.good();
}
