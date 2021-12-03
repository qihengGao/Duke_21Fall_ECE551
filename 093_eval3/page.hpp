#ifndef __PAGE_HPP__
#define __PAGE_HPP__

#include <cstdlib>
#include <string>
#include <vector>

/* Page Class */
class Page {
 private:
  /* Page number of this page. */
  size_t pageNum;

  /*  Stores every line of text in order in a vector. */
  std::vector<std::string> text;

  /* If the page is the normal choice navigation page, it contains the information
   * of the next page number and their corresponding choices.
   */
  std::vector<size_t> nextPagesNum;
  std::vector<std::string> choices;

  /* To indicate if current page object is an ending (WIN/LOSE) 
   * or a normal navigation page.
   */
  bool isWinPage;
  bool isLosePage;

  /* private methods of  parsing input for object construction. */
  void parsePage(const std::string & fileName);
  void parseChoice(const std::string & readIn);

 public:
  /* Default constructor. */
  Page() : pageNum(-1), isWinPage(false), isLosePage(false) {}

  explicit Page(const std::string & fileName) :
      pageNum(-1),
      isWinPage(false),
      isLosePage(false) {
    this->parsePage(fileName);
  }
  /* Assignment. */
  Page & operator=(const Page & rhs);
  /* Destructor. */
  ~Page() {}

  /* Setters. */
  static size_t parsePageNum(const std::string & pageNumSt);
  void setPageNum(const std::string & fileName);
  void addText(const std::string & text);
  void addNextPagesNum(const size_t pageNum);
  void addChoices(const std::string & choice);
  void setAsWin();
  void setAsLose();

  /* Getters. */
  size_t getPageNum() const;
  size_t choiceRange() const;
  const std::vector<std::string> & getText() const;
  const std::vector<size_t> & getNextPagesNum() const;
  const std::vector<std::string> & getChoices() const;
  bool isWin() const;
  bool isLose() const;
  bool isChoicePage() const;

  void printText() const;
  void printChoices() const;
  void printPage() const;
};
#endif
