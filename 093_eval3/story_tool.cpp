#include "story_tool.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>

/* parsePage parses the text given in the file named 'fileName'
 * and modifies the page passed in. 
 *
 * @param page, the reference to the Page object to be modified
 * @param fileName, a string, the name of txt file
 */
void parsePage(Page & page, std::string & fileName) {
  std::ifstream file(fileName);
  if (file.fail()) {
    std::cerr << "Cannot open: " << fileName << std::endl;
    exit(EXIT_FAILURE);
  }

  /* An indicator to split page processing to choices or text. */
  bool processText = false;
  /* Count the line number during process. */
  size_t lineNum = 0;
  std::string readIn;
  while (getline(file, readIn)) {
    ++lineNum;
    /* Segmentation occurs, start processing text on the next line. */
    if (readIn[0] == '#' && !processText) {
      /* Page is the ending page but separation does not occur at right position. */
      if ((page.isWin() || page.isLose()) && lineNum != 2) {
        std::cerr << "Segmentation does not occur right after the WIN/LOSE line."
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      processText = true;
      continue;
    }

    /* WIN/LOSE occurs at the first line. Then we perceive the page as WIN/LOSE. */
    if (lineNum == 1 && readIn == "WIN") {
      page.setAsWin();
      continue;
    }
    else if (lineNum == 1 && readIn == "LOSE") {
      page.setAsLose();
      continue;
    }

    if (processText) {
      page.addText(readIn);
    }
    else {
      parseChoice(page, readIn);
    }
  }

  if (!file.eof()) {
    std::cerr << "Have trouble in reading " << fileName << std::endl;
  }
}

/* parseChoice parses the choice line into the proper field of page object.
 * To be more detailed, append the page number to .nextPages
 * and append the choice content to .Choices
 * @param page, the reference to the page object to be parsed
 * @param, string, the choice line read in
 */
void parseChoice(Page & page, std::string & readIn) {
  size_t colon = readIn.find(":");
  if (colon == std::string::npos) {
    std::cerr << "Colon is not found in current choice line." << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Parse content before the 1st colon as next page number. */
  page.addNextPages(parseNextPage(readIn.substr(0, colon)));
  /* Take the content after 1st colon as the choice. */
  page.addChoices(readIn.substr(colon + 1));
}

/* parseNextpage takes a string of nextPageNumber and return a number in size_t.
 * indicates and exits if any error occurs.
 * @param nextPageStr, a string of nextPageNumber
 */
size_t parseNextPage(const std::string & nextPageStr) {
  if (nextPageStr.length() == 0) {
    std::cerr << "Next page number is not present." << std::endl;
    exit(EXIT_FAILURE);
  }

  if (nextPageStr.find("-") != std::string::npos) {
    std::cerr << "Contains negative sign in the nextPage section." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::istringstream builder(nextPageStr);
  size_t nextPageNum;
  builder >> nextPageNum;
  if (!builder.eof()) {
    std::cerr << nextPageStr << "Cannot convert entirely to a size_t number."
              << std::endl;
    exit(EXIT_FAILURE);
  }
  if (nextPageNum == 0) {
    std::cerr << "Next Page Number should be strictly greater than 0." << std::endl;
    exit(EXIT_FAILURE);
  }
  return nextPageNum;
}

void printPage(Page & page) {
  printText(page);
  /* WIN Page. */
  if (page.isWin()) {
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
  }
  /* Lost Page. */
  else if (page.isLose()) {
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
  }
  else {
    printChoices(page);
  }
}

void printText(Page & page) {
  std::vector<std::string> text = page.getText();
  for (size_t i = 0; i < text.size(); i++) {
    std::cout << text[i] << std::endl;
  }
}

void printChoices(Page & page) {
  std::vector<std::string> choices = page.getChoices();
  if (choices.size() == 0) {
    std::cerr << "No choice available." << std::endl;
  }
  std::cout << std::endl;
  std::cout << "What would you like to do?" << std::endl;
  std::cout << std::endl;
  for (size_t i = 0; i < choices.size(); i++) {
    std::cout << " " << i + 1 << ". " << choices[i] << std::endl;
  }
}
