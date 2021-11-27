#include "story.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include <stack>

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
bool fileIsExist(const std::string & filePath) {
  std::ifstream file(filePath.c_str());
  return file.good();
}

/* Story constructor. */
Story::Story(const std::string & directory) {
  std::stringstream filePath;
  size_t pageNumber = 1;
  generateFileName(filePath, directory, pageNumber);
  /* While the file in the path can be openned. */
  while (fileIsExist(filePath.str())) {
    this->addPage(Page(filePath.str()));
    this->pageNumberSet.insert(pageNumber);
    /* Increse page number and generate next file path. */
    pageNumber++;
    generateFileName(filePath, directory, pageNumber);
  }
  if (pageNumber == 1) {
    std::cerr << "page1.txt does not exist. " << std::endl;
    exit(EXIT_FAILURE);
  }
}

/* Add page object to story. */
void Story::addPage(const Page & page) {
  this->pages.push_back(page);
}

/* Check choices in each page are valid in range. */
void Story::checkChoices() const {
  /* Iterate on the page. */
  for (size_t i = 0; i < this->pages.size(); i++) {
    /* Get the list of the next page number for the current page i. */
    const std::vector<size_t> nextPagesOfCurr = this->pages[i].getNextPagesNum();
    /* Iterate the list of the next page number to check if it is contained. */
    for (size_t j = 0; j < nextPagesOfCurr.size(); j++) {
      if (this->pageNumberSet.find(nextPagesOfCurr[j]) == this->pageNumberSet.end()) {
        std::cerr << "Choices page number is over current available page number."
                  << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

/* Check each page is at least referenced by one page. */
void Story::checkPageConnect() const {
  /* Initialize a vector of bool to indicate if a page is reachable,
   * with default values: false.
   */
  std::vector<bool> referenced(this->pages.size(), false);
  /* Iterate on the page. */
  for (size_t i = 0; i < this->pages.size(); i++) {
    /* Get the list of the next page number for the current page i. */
    const std::vector<size_t> nextPagesOfCurr = this->pages[i].getNextPagesNum();
    /* Iterate the list of the next page number to set reachable status as true.  */
    for (std::vector<size_t>::const_iterator it = nextPagesOfCurr.begin();
         it != nextPagesOfCurr.end();
         ++it) {
      referenced[*it - 1] = true;  // 0-based
    }
  }

  /* Skip page 1, check if the rest is referenced. */
  for (size_t i = 1; i < referenced.size(); i++) {
    if (!referenced[i]) {
      std::cerr << "page" << i << "is not refereced." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

/* Check the story has both win and lose pages. */
void Story::checkHasWinLose() const {
  /* Set two marker to record the occurence of win and lose page. */
  bool hasWin = false;
  bool hasLose = false;
  /* Iterate on the pages to check if there have win and lose page. */
  for (size_t i = 0; i < this->pages.size(); i++) {
    if (this->pages[i].isWin()) {
      hasWin = true;
    }
    else if (this->pages[i].isLose()) {
      hasLose = true;
    }
  }
  if (!hasWin) {
    std::cerr << "WIN page misses." << std::endl;
    exit(EXIT_FAILURE);
  }
  else if (!hasLose) {
    std::cerr << "LOSE page misses." << std::endl;
    exit(EXIT_FAILURE);
  }
}

/* Combine the checks in one. */
void Story::check() const {
  this->checkChoices();
  this->checkPageConnect();
  this->checkHasWinLose();
}

/* Helper function. To check if user input is in range. */
bool userChoiceInRange(size_t userChoice, size_t choiceRange) {
  return userChoice > 0 && userChoice <= choiceRange;
}

void Story::start() const {
  /* Starts from page 1. */
  size_t nextPageNum = 1;
  Page nextPage = this->pages[nextPageNum - 1];

  /* Start adventrue. */
  while (nextPage.isChoicePage()) {
    nextPage.printPage();
    /* Get the choice range of current page for further unser input check. */
    size_t choiceRange = nextPage.choiceRange();
    size_t userChoice = 0;
    // bool waitInput = true;
    std::string readIn;
    /* Get user input until user input is valid. */
    while (getline(std::cin, readIn)) {
      // std::cin >> userChoice;
      std::stringstream builder(readIn);
      builder >> userChoice;
      if (!builder.eof() || !userChoiceInRange(userChoice, choiceRange)) {
        std::cerr << "That is not a valid choice, please try again" << std::endl;
        std::cin.clear();
      }
      else {
        break;
      }
    }

    /* Jump to the next page based on user choice. */
    nextPageNum = nextPage.getNextPagesNum()[userChoice - 1];
    nextPage = this->pages[nextPageNum - 1];
  }

  /* Print Ending. */
  nextPage.printPage();
}

/* Step3 print the depths of pages. */
void Story::printPagesDepth() const {
  /* Initialize visited vector to circumvent duplicated visiting. */
  std::vector<bool> visited(this->pages.size(), false);
  /* To store the depth information of each page starting from page1. */
  std::vector<size_t> depths(this->pages.size(), 0);
  countDepths(this->pages[0], visited, depths);
  for (size_t i = 1; i <= this->pages.size(); i++) {
    std::cout << "Page " << i;
    if (!visited[i - 1]) {
      std::cout << " is not reachable" << std::endl;
    }
    else {
      std::cout << ":" << depths[i - 1] << std::endl;
    }
  }
}

/* Step 4 get all possible paths to win. */
void Story::showPathToWin() const {
  std::vector<bool> visited(this->pages.size(), false);
  /* Store the path to win and its corresponding choices. */
  std::vector<std::pair<std::vector<Page>, std::vector<size_t> > > result;
  collectPaths(this->pages[0], visited, result);
  /* No path to win. */
  if (result.empty()) {
    std::cout << "This story is unwinnable!" << std::endl;
    return;
  }
  /* Print pages before the win. */
  for (size_t i = 0; i < result.size(); i++) {
    std::vector<Page> pathToWin = result[i].first;
    std::vector<size_t> choicesToWin = result[i].second;
    for (size_t j = 0; j < choicesToWin.size(); j++) {
      std::cout << pathToWin[j].getPageNum() << "(" << choicesToWin[j] << ")"
                << ",";
    }
    /* Print win. */
    std::cout << pathToWin[pathToWin.size() - 1].getPageNum() << "(win)" << std::endl;
  }
}

/* Carry out breadth-first-search to traverse the page level by level
 * to get the depths.
 */
void Story::countDepths(const Page & start,
                        std::vector<bool> & visited,
                        std::vector<size_t> & depths) const {
  std::queue<Page> queue;
  /* Push the start to the queue. */
  queue.push(start);
  /* Starting with 0 depth. */
  size_t depth = 0;
  while (!queue.empty()) {
    size_t size = queue.size();
    /* Pages in the same level are in the same depth. */
    for (size_t i = 0; i < size; i++) {
      Page currPage = queue.front();
      queue.pop();
      depths[currPage.getPageNum() - 1] = depth;
      visited[currPage.getPageNum() - 1] = true;
      std::vector<size_t> nextPagesNum = currPage.getNextPagesNum();
      for (size_t c = 1; c <= currPage.choiceRange(); c++) {
        if (!visited[nextPagesNum[c - 1] - 1]) {
          queue.push(this->pages[nextPagesNum[c - 1] - 1]);
        }
      }
    }
    depth++;
  }
}

/* Carry out depth first search to collect the paths to win by implementing stack. */
void Story::collectPaths(
    const Page & start,
    std::vector<bool> & visited,
    std::vector<std::pair<std::vector<Page>, std::vector<size_t> > > & result) const {
  /* Record the path during the process. */
  std::vector<Page> path;

  /* Record the choices during the process. */
  std::vector<size_t> choice;
  path.push_back(start);

  std::stack<std::vector<Page> > stack;

  /* Record the corresponding visited status for each. */
  std::stack<std::vector<bool> > visitedStatus;

  /* Record the corresponding choice status for each. */
  std::stack<std::vector<size_t> > choices;
  stack.push(path);
  visitedStatus.push(visited);
  choices.push(choice);

  while (!stack.empty()) {
    std::vector<Page> currPath = stack.top();
    std::vector<bool> currVisited = visitedStatus.top();
    std::vector<size_t> currChoice = choices.top();
    stack.pop();
    visitedStatus.pop();
    choices.pop();
    Page currPage = currPath.back();
    /* If current page is win, add to result. */
    if (currPage.isWin()) {
      result.push_back(
          std::pair<std::vector<Page>, std::vector<size_t> >(currPath, currChoice));
    }
    /* If haven't get to the win, update visired status and continue traversing. */
    currVisited[currPage.getPageNum() - 1] = true;
    std::vector<size_t> nextPagesNum = currPage.getNextPagesNum();
    for (size_t c = 1; c <= currPage.choiceRange(); c++) {
      if (!currVisited[nextPagesNum[c - 1] - 1]) {
        currPath.push_back(this->pages[nextPagesNum[c - 1] - 1]);
        currChoice.push_back(c);
        stack.push(currPath);
        visitedStatus.push(currVisited);
        choices.push(currChoice);
        currPath.pop_back();
        currChoice.pop_back();
      }
    }
  }
}
