#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include "provided.h"
struct string_tag {
  char * content;
  size_t len;
};
typedef struct string_tag string_t;

string_t * initStringT();
void freeStringT(string_t * str);

void appendChars(string_t ** dest, const char * src, size_t srcLen);
void appendStringT(string_t ** dest, string_t * src);

int checkBlank(char * line, ssize_t readLen);
string_t * parseLine(char * line, ssize_t readLen);
char * getCategory(char * start, size_t categoryLen);
#endif
