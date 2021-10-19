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

void appendChars(string_t * dest, const char * src, size_t srcLen) {
  dest->content =
      realloc(dest->content, (dest->len + srcLen + 1) * sizeof(*dest->content));
  memcpy(dest->content + dest->len, src, srcLen);
  dest->len += srcLen;
  dest->content[dest->len] = '\0';
}
void appendStringT(string_t * dest, string_t * src) {
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

string_t * parseBlank(char * line, ssize_t readLen) {
  string_t * result = initStringT();

  char * start = line;
  char * end = line + readLen;

  while (start < end) {
    char * blankStart = strchr(start, '_');
    if (blankStart == NULL) {
      appendChars(result, start, strlen(start));
      return result;
    }
    if (blankStart > start) {
      appendChars(result, start, blankStart - start);
    }
    char * blankEnd = strchr(blankStart + 1, '_');
    char * categoryStart = blankStart + 1;
    size_t categoryLen = blankEnd - blankStart - 1;
    char * category = strndup(categoryStart, categoryLen);
    const char * word = chooseWord(category, NULL);
    free(category);
    appendChars(result, word, strlen(word));
    start = blankEnd + 1;
  }
  return result;
}

category_t initCategory() {
  category_t result = {.name = NULL, .words = NULL, .n_words = 0};
  return result;
}

catarray_t * initCatArray() {
  catarray_t * result = malloc(sizeof(*result));
  result->arr = NULL;
  result->n = 0;
  return result;
}

char * getName(char * nameStart, char * colon) {
  size_t nameLen = colon - nameStart;
  char * name = strndup(nameStart, nameLen);
  return name;
}

char * getWord(char * wordStart, char * wordEnd) {
  size_t wordLen = wordEnd - wordStart;
  char * nextLine = strchr(wordStart, '\n');
  if (nextLine != NULL) {
    wordLen -= 1;
  }
  char * word = strndup(wordStart, wordLen);
  return word;
}

void addCategory(catarray_t * catArray, char * name, char * word) {
  category_t * category = getCategory(catArray, name);
  if (category != NULL) {
    appendCategory(category, word);
  }
  else {
    addNewCategory(catArray, name, word);
  }
}

category_t * getCategory(catarray_t * catArray, char * name) {
  for (size_t i = 0; i < catArray->n; i++) {
    if (strcmp(name, catArray->arr[i].name) == 0) {
      return &(catArray->arr[i]);
    }
  }
  return NULL;
}

void appendCategory(category_t * category, char * word) {
  category->words =
      realloc(category->words, (category->n_words + 1) * sizeof(*category->words));
  category->words[category->n_words] = strdup(word);
  category->n_words++;
}

void addNewCategory(catarray_t * catArray, char * name, char * word) {
  catArray->arr = realloc(catArray->arr, (catArray->n + 1) * sizeof(*catArray->arr));
  catArray->arr[catArray->n] = initCategory();
  catArray->arr[catArray->n].name = strdup(name);
  appendCategory(&(catArray->arr[catArray->n]), word);
  catArray->n++;
}

void freeCategory(category_t category) {
  for (size_t i = 0; i < category.n_words; i++) {
    free(category.words[i]);
  }
  free(category.words);
  free(category.name);
}

void freeCatArray(catarray_t * catArray) {
  for (size_t i = 0; i < catArray->n; i++) {
    freeCategory(catArray->arr[i]);
  }
  free(catArray->arr);
  free(catArray);
}
