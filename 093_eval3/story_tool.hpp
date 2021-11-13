#ifndef __STORY_TOOL_HPP__
#define __STORY_TOOL_HPP__
#include "page.hpp"

void parsePage(Page & page, const std::string & fileName);
void parseChoice(Page & page, const std::string & readIn);
size_t parseNextPage(const std::string & nextPageStr);
void printPage(Page & page);
void printText(Page & page);
void printChoices(Page & page);
#endif
