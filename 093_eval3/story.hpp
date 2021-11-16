#ifndef __STORY_HPP__
#define __STORY_HPP__

#include <vector>

#include "page.hpp"

class Story {
 private:
  std::vector<Page> pages;

 public:
  Story() {}
  explicit Story(const std::string & directory);

  void addPage(const Page & page);
};

#endif
