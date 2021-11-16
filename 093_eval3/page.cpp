#include "page.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>

/* parseNextpage takes a string of nextPageNumber and return a number in size_t.
 * indicates and exits if any error occurs.
 * @param nextPageStr, a string of nextPageNumber
 */
size_t parsePageNum(const std::string & pageNumStr) {
  if (pageNumStr.length() == 0) {
    std::cerr << "Page number is not present." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (pageNumStr.find("-") != 0) {
    std::cerr << "Contains negative sign." << std::endl;
    exit(EXIT_FAILURE);
  }
  std::istringstream builder(pageNumStr);
  size_t pageNumber;
  builder >> pageNumber;
  if (!builder.eof()) {
    std::cerr << pageNumStr << " Cannot convert entirely to a size_t number."
              << std::endl;
    exit(EXIT_FAILURE);
  }
  if (pageNumber <= 0) {
    std::cerr << "Next Page Number should be strictly greater than 0." << std::endl;
    exit(EXIT_FAILURE);
  }
  return pageNumber;
}

/* parsePage parses the text given in the file named 'fileName'
 * and modifies the page passed in. 
 *
 * @param fileName, a string reference to the name of txt file
 */
void Page::parsePage(const std::string & fileName) {
  this->setPageNum(fileName);

  std::ifstream file(fileName.c_str());
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
    if (readIn.length() > 0 && readIn[0] == '#' && !processText) {
      /* Page is the ending page but separation does not occur at right position. */
      if ((!this->isChoicePage()) && lineNum != 2) {
        std::cerr << "Segmentation does not occur right after the WIN/LOSE line."
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      processText = true;
      continue;
    }

    /* WIN/LOSE occurs at the first line. Then we perceive the page as WIN/LOSE. */
    if (lineNum == 1 && readIn == "WIN") {
      this->setAsWin();
      continue;
    }
    else if (lineNum == 1 && readIn == "LOSE") {
      this->setAsLose();
      continue;
    }
    /* Else add text or add choice part according to the status indicator. */
    if (processText) {
      this->addText(readIn);
    }
    else {
      this->parseChoice(readIn);
    }
  }

  if (!file.eof()) {
    std::cerr << "Have trouble in reading " << fileName << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Check if choice is present in normal navigation page. */
  if (this->isChoicePage() && this->choices.empty()) {
    std::cerr << "Contents miss in " << fileName << std::endl;
    exit(EXIT_FAILURE);
  }
}

/* parseChoice parses the choice line into the proper field of page object.
 * To be more detailed, append the page number to .nextPages
 * and append the choice content to .Choices
 *
 * @param, string, the choice line read in
 */
void Page::parseChoice(const std::string & readIn) {
  size_t colon = readIn.find(":");
  if (colon == std::string::npos) {
    std::cerr << "Colon is not found in current choice line." << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Parse content before the 1st colon as next page number. */
  this->addNextPagesNum(parsePageNum(readIn.substr(0, colon)));
  /* Take the content after 1st colon as the choice. */
  this->addChoices(readIn.substr(colon + 1));
}

/* Adders/Setters: modify the field of a page object. */
void Page::setPageNum(const std::string & fileName) {
  size_t pagePos = fileName.find("page");
  size_t dotPos = fileName.find(".txt");
  /* If fileName is not matched. */
  if (pagePos == std::string::npos || dotPos == std::string::npos) {
    std::cerr << fileName << " does not match the format: page__.txt" << std::endl;
    exit(EXIT_FAILURE);
  }
  size_t numStart = pagePos + 4;
  size_t numLen = dotPos - numStart;
  /* skip "page" and end brefore the dot */
  this->pageNum = parsePageNum(fileName.substr(numStart, numLen));
}

/* Adders/Setters modify a specific feild of page object. */
void Page::addText(const std::string & text) {
  this->text.push_back(text);
}

void Page::addNextPagesNum(const size_t pageNum) {
  this->nextPagesNum.push_back(pageNum);
}

void Page::addChoices(const std::string & choice) {
  this->choices.push_back(choice);
}

void Page::setAsWin() {
  this->isWinPage = true;
}

void Page::setAsLose() {
  this->isLosePage = true;
}

/* Getters: get the the field of a page object. */
size_t Page::getPageNum() const {
  return this->pageNum;
}
const std::vector<std::string> & Page::getText() const {
  return this->text;
}

const std::vector<size_t> & Page::getNextPagesNum() const {
  return this->nextPagesNum;
}

const std::vector<std::string> & Page::getChoices() const {
  return this->choices;
}

bool Page::isWin() const {
  return this->isWinPage;
}

bool Page::isLose() const {
  return this->isLosePage;
}

bool Page::isChoicePage() const {
  return (!this->isWinPage && !this->isLosePage);
}

/* printPage print the required format of the page according to the type of page object,
 * i.e. choice, WIN or LOSE.
 */
void Page::printPage() const {
  this->printText();
  std::cout << std::endl;
  /* WIN Page. */
  if (this->isWin()) {
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
  }
  /* Lose Page. */
  else if (this->isLose()) {
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
  }
  else {
    this->printChoices();
  }
}

/* printText prints the text stored in text vector line by line. */
void Page::printText() const {
  const std::vector<std::string> text = this->getText();
  for (size_t i = 0; i < text.size(); i++) {
    std::cout << text[i] << std::endl;
  }
}

/* printChoices prints the choices stored in the choices vector. */
void Page::printChoices() const {
  const std::vector<std::string> choices = this->getChoices();
  std::cout << "What would you like to do?" << std::endl;
  std::cout << std::endl;
  for (size_t i = 0; i < choices.size(); i++) {
    std::cout << " " << i + 1 << ". " << choices[i] << std::endl;
  }
}
