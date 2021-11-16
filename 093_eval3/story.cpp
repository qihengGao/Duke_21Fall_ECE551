#include "story.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "page.hpp"
/* Helper Functions*/

/* generateFilename combines the directory and pagesNumber to a page file path.
 * @param filePath, a reference to a stringstream to be appended
 * @param directory, input directory
 * @param pageNumber, the pageNumber to be appended to the page file path
 */
void generateFileName(std::stringstream & filePath,
                      const std::string & directory,
                      size_t pageNumber) {
  /* Clear the stingstream and reset its status for new file path generation*/
  filePath.str(std::string());
  filePath.clear();
  filePath << directory << "/page" << pageNumber << ".txt";
}

/* isFileExist checks if the file given in the filePath really exists.
 * @param filePath
 * @return a boolean to indicate if the the file can be openned, i.e. exists
 */
bool isFileExist(const std::string & filePath) {
  std::ifstream file(filePath.c_str());
  return file.good();
}

/* Story constructor. */
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

void Story::addPage(const Page & page) {
  this->pages.push_back(page);
}
