#include "rand_story.h"

#include <string.h>

#include "provided.h"

string_t * initStringT() {
  string_t * result = malloc(sizeof(*result));
  result->content = malloc(sizeof(*result->content));
  *result->content = '\0';
  result->len = 0;
  return result;
}
void freeStringT(string_t * str) {
  free(str->content);
  free(str);
}

void appendChars(string_t ** dest, const char * src, size_t srcLen) {
  (*dest)->content = realloc((*dest)->content,
                             (((*dest)->len + srcLen + 1) * sizeof(*(*dest)->content)));
  memcpy((*dest)->content + (*dest)->len, src, srcLen);
  (*dest)->len += srcLen;
  (*dest)->content[(*dest)->len] = '\0';
}
void appendStringT(string_t ** dest, string_t * src) {
  appendChars(dest, src->content, src->len);
}

int checkBlank(char * line, ssize_t readLen) {
  char * start = line;
  char * end = start + readLen;
  size_t count = 0;
  while (start != NULL && start < end) {
    start = strchr(start, '_');
    if (start != NULL) {
      count++;
      start++;
    }
  }
  return (count % 2 == 0) ? 1 : 0;
}

string_t * parseLine(char * line, ssize_t readLen) {
  string_t * result = initStringT();

  char * start = line;
  char * end = line + readLen;

  while (start < end) {
    char * blankStart = strchr(start, '_');
    if (blankStart == NULL) {
      appendChars(&result, start, strlen(start));
      return result;
    }
    if (blankStart > start) {
      appendChars(&result, start, blankStart - start);
    }
    char * blankEnd = strchr(blankStart + 1, '_');
    char * category = getCategory(blankStart + 1, blankEnd - blankStart - 1);
    const char * word = chooseWord(category, NULL);
    free(category);
    appendChars(&result, word, strlen(word));
    start = blankEnd + 1;
  }
  return result;
}

char * getCategory(char * start, size_t categoryLen) {
  char * category = malloc((categoryLen + 1) * sizeof(*category));
  memcpy(category, start, categoryLen);
  category[categoryLen] = '\0';
  return category;
}
