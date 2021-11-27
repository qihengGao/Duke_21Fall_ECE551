#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <set>
#include <vector>

#include "page.hpp"

class Story {
 private:
  std::vector<Page> pages;
  std::set<size_t> pageNumberSet;
  /* Check choices in each page are valid in range. */
  void checkChoices() const;
  /* Check each page is at least referenced by one page. */
  void checkPageConnect() const;
  /* Check the story has both win and lose pages. */
  void checkHasWinLose() const;
  void countDepths(const Page & start,
                   std::vector<bool> & visited,
                   std::vector<size_t> & depths) const;

  void collectPaths(
      const Page & start,
      std::vector<bool> & visited,
      std::vector<std::pair<std::vector<Page>, std::vector<size_t> > > & result) const;

 public:
  Story() {}
  explicit Story(const std::string & directory);

  void addPage(const Page & page);
  /* Combine the following check in one. */
  void check() const;
  /* Start the adventrue. */
  void start() const;

  void printPagesDepth() const;
  void showPathToWin() const;
};

#endif
