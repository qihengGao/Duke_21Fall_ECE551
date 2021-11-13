#ifndef __PAGE_HPP__
#define __PAGE_HPP__

#include <cstdlib>
#include <string>
#include <vector>

/* Page Class */
class Page {
 private:
  size_t pageNum;
  /*  Stores every line of text in order in a vector. */
  std::vector<std::string> text;

  /* Elements in nextPagesNum and choices have the one-to-one relationship
   * i.e. nextPagesNum[i] : choices[i]
   */
  std::vector<size_t> nextPagesNum;
  std::vector<std::string> choices;

  /* To indicate if current page object is an ending (WIN/LOSE) 
   * or a normal navigation page.
   */
  bool isWinPage;
  bool isLosePage;

  void parsePage(const std::string & fileName);
  void parseChoice(const std::string & readIn);
  size_t parsePageNum(const std::string & pageStr);

 public:
  Page() : pageNum(-1), isWinPage(false), isLosePage(false) {}
  explicit Page(const std::string & fileName) :
      pageNum(-1),
      isWinPage(false),
      isLosePage(false) {
    this->parsePage(fileName);
  }
  ~Page() {}

  void setPageNum(const std::string & fileName);
  void addText(const std::string & text);
  void addNextPagesNum(const size_t pageNum);
  void addChoices(const std::string & choice);
  void setAsWin();
  void setAsLose();

  size_t getPageNum();
  std::vector<std::string> & getText();
  std::vector<size_t> & getNextPagesNum();
  std::vector<std::string> & getChoices();
  bool isWin();
  bool isLose();

  void printText();
  void printChoices();
  void printPage();
};
#endif
