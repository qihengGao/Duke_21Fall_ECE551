#include "rand_story.h"

#include <string.h>

#include "provided.h"

/* Step 1 */
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

string_t * parseLine(char * line,
                     ssize_t readLen,
                     category_t * used,
                     catarray_t * catArray,
                     int reused) {
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
    char * word = parseBlank(blankStart, blankEnd, used, catArray, reused);
    appendCategory(used, word);
    appendChars(result, word, strlen(word));
    start = blankEnd + 1;
    free(word);
  }
  return result;
}

char * parseBlank(char * blankStart,
                  char * blankEnd,
                  category_t * used,
                  catarray_t * catArray,
                  int reused) {
  char * word = NULL;
  char * categoryStart = blankStart + 1;
  size_t categoryLen = blankEnd - blankStart - 1;
  char * category = strndup(categoryStart, categoryLen);
  char * endPtr = NULL;
  size_t number = strtoull(category, &endPtr, 10);

  if (*endPtr == '\0') {
    if (number <= 0 || number > used->n_words) {
      fprintf(stderr, "Number is not valid.\n");
      exit(EXIT_FAILURE);
    }
    word = strdup(used->words[used->n_words - number]);
  }
  else {
    if (catArray != NULL && !getCategory(catArray, category)) {
      fprintf(stderr, "%s is not a valid category.\n", category);
      exit(EXIT_FAILURE);
    }
    const char * chosenWord = chooseWord(category, catArray);
    word = strdup(chosenWord);
    if (reused == 0) {
      removeWord(catArray, category, chosenWord);
    }
  }

  free(category);
  return word;
}

/* Step 2 */
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

void removeWord(catarray_t * catArray, char * name, const char * word) {
  category_t * category = getCategory(catArray, name);
  size_t idxOfDel = locateWord(category, word);
  if (idxOfDel == category->n_words) {
    return;
  }
  char * ptrToDel = category->words[idxOfDel];
  free(ptrToDel);
  for (size_t i = idxOfDel; i < category->n_words - 1; i++) {
    category->words[i] = category->words[i + 1];
  }
  category->n_words--;
}

size_t locateWord(category_t * category, const char * word) {
  size_t result = 0;
  for (size_t i = 0; i < category->n_words; i++) {
    if (strcmp(category->words[i], word) == 0) {
      result = i;
      return result;
    }
  }
  return result;
}

category_t * getCategory(catarray_t * catArray, char * name) {
  for (size_t i = 0; i < catArray->n; i++) {
    if (strcmp(name, catArray->arr[i].name) == 0) {
      return &(catArray->arr[i]);
    }
  }
  return NULL;
}

void appendCategory(category_t * category, const char * word) {
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

/* Step 3 */
void tellStory(char * catWordFile, char * storyTemplate, int reused) {
  catarray_t * catArray = getCatArray(catWordFile);
  string_t * story = getStory(storyTemplate, catArray, reused);

  printf("%s", story->content);

  freeStringT(story);
  freeCatArray(catArray);
}

catarray_t * getCatArray(char * catWordFile) {
  FILE * f = fopen(catWordFile, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot oppen file: %s.\n", catWordFile);
    exit(EXIT_FAILURE);
  }

  catarray_t * catArray = initCatArray();
  char * line = NULL;
  size_t bufferSize = 0;
  ssize_t readLen = 0;
  while ((readLen = getline(&line, &bufferSize, f)) != -1) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "Input line does not have the colon as a separator.\n");
      exit(EXIT_FAILURE);
    }
    char * name = getName(line, colon);
    char * word = getWord(colon + 1, line + readLen);
    addCategory(catArray, name, word);
    free(name);
    free(word);
  }
  free(line);
  if (fclose(f) == EOF) {
    fprintf(stderr, "Cannot close file.\n");
    exit(EXIT_FAILURE);
  }
  return catArray;
}

string_t * getStory(char * storyTemplate, catarray_t * catArray, int reused) {
  FILE * f = fopen(storyTemplate, "r");
  if (f == NULL) {
    fprintf(stderr, "File: %s does not exist.\n", storyTemplate);
    exit(EXIT_FAILURE);
  }

  string_t * parsedStory = initStringT();
  category_t used = initCategory();

  char * line = NULL;
  size_t bufferSize = 0;
  ssize_t readLen = 0;
  while ((readLen = getline(&line, &bufferSize, f)) != -1) {
    if (checkBlank(line, readLen) == 0) {
      fprintf(stderr,
              "The blank does not have matching closed underscore in the same line.\n");
      exit(EXIT_FAILURE);
    }
    string_t * prunedLine = parseLine(line, readLen, &used, catArray, reused);
    appendStringT(parsedStory, prunedLine);
    freeStringT(prunedLine);
  }
  free(line);
  freeCategory(used);
  if (fclose(f) == EOF) {
    fprintf(stderr, "Cannot close file.\n");
    exit(EXIT_FAILURE);
  }
  return parsedStory;
}
