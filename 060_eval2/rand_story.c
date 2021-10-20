#include "rand_story.h"

#include <string.h>

#include "provided.h"

/* initStringT() initialize the fileds of self-defined struct string_t,
 * .content = '\0', .len = 0
 * @return a pointer to the initialized one.
 */
string_t * initStringT() {
  string_t * result = malloc(sizeof(*result));
  result->content = malloc(sizeof(*result->content));
  *result->content = '\0';
  result->len = 0;
  return result;
}

/* freeStringT frees the memory used in the string_t.
 * @param str, a pointer to string_t *
 */
void freeStringT(string_t * str) {
  free(str->content);
  free(str);
}

/* appendChars appends the given array of characters to the destination.
 * @param dest, a pointer to a string_t struct which is to be appended
 * @param src, a pointer to an array of characters to append (source)
 * @param srcLen, the length of source
 */
void appendChars(string_t * dest, const char * src, size_t srcLen) {
  dest->content =
      realloc(dest->content, (dest->len + srcLen + 1) * sizeof(*dest->content));
  memcpy(dest->content + dest->len, src, srcLen);
  dest->len += srcLen;
  dest->content[dest->len] = '\0';
}

/* appendStringT appends the content in second string_t struct to the first one,
 * and internally calls appendChars.
 * @param dest, a pointer to a string_t which is to be appended
 * @param src, a pointer to a string_t,
 *        and its content would be appended to the dest->content
 */
void appendStringT(string_t * dest, string_t * src) {
  appendChars(dest, src->content, src->len);
}

/* checkBlank checks if the underscores in a line are all matched.
 * @param line, a pointer to a string which is to be checked
 * @param readLen, the length to be checked, 
 *        and it is always the total length of the line in this project 
 * @return 1, the underscores are all mactched
 *         0, indicates a mistacked situation
 */
int checkBlank(char * line, ssize_t readLen) {
  /* Iterate on the line and count the number of underscores,
   * and the underscores are matched iff the number is even.
   */
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

/* parseLine processes the read-in line subject to the functionality requirement. 
 * @param line, a pointer to the read-in string
 * @param readLen, the length of the string
 * @param used, a pointer to a category_t which stores 
 *        the used words in the whole story generation processes
 * @param catArray, a pointer to catarray_t, which is utilized in choosing word
 *        with respect to the category in the blank
 * @param reused, an indicator, 
 *        1 can resue preiously used word, 
 *        0 forbid useing duplicate words in the same category
 * @return a pointer to a pruned line (string_t)
 */
string_t * parseLine(char * line,
                     ssize_t readLen,
                     category_t * used,
                     catarray_t * catArray,
                     int reused) {
  string_t * result = initStringT();
  /* start inidicates the current position to be processed. */
  char * start = line;
  /* end inidcates where the proccess should be finally stop. */
  char * end = line + readLen;

  while (start < end) {
    char * blankStart = strchr(start, '_');

    /* If there is no blank left,
     *  then just append the rest line to the result and return.
     */
    if (blankStart == NULL) {
      appendChars(result, start, strlen(start));
      return result;
    }

    /* There exits several words/characters between the blank and start,
     * then append them.
     */
    if (blankStart > start) {
      appendChars(result, start, blankStart - start);
    }
    char * blankEnd = strchr(blankStart + 1, '_');
    /* Call parseBlank. */
    char * word = parseBlank(blankStart, blankEnd, used, catArray, reused);
    appendCategory(used, word);
    appendChars(result, word, strlen(word));
    /* Move forward the start pointer to the next character to be proccessed. */
    start = blankEnd + 1;
    free(word);
  }
  return result;
}

/* parseBlank processes the blank subject to the functionality requirement
 * (number or a category), and returns the proper word,
 * indicates any erroes if it encounters and then exits.
 * @param blankStart, a postion where the blank starts
 * @param blankEnd, a postion where the blank ends
 * @param used, a pointer to a category_t which stores 
 *        the used words in the whole story generation processes
 * @param catArray, a pointer to catarray_t, which is utilized in choosing word
 *        with respect to the category in the blank
 * @param reused, an indicator, 
 *        1 can resue preiously used word, 
 *        0 forbid useing duplicate words in the same category
 * @return word, a string that satisfies the functionality requirement
 */
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

  /* If there is a 'pure' number in the blank.
   * i.e. this is not a null string, and strtoull reads till the end.
   */
  if (categoryLen != 0 && *endPtr == '\0') {
    /* Check if out of range. */
    if (number <= 0 || number > used->n_words) {
      fprintf(stderr, "Number is not valid.\n");
      exit(EXIT_FAILURE);
    }
    word = strdup(used->words[used->n_words - number]);
  }
  /* Otherwise, the string in blank is regarded as a category. */
  else {
    /* Check if the category exits in the array of category. */
    if (catArray != NULL && !getCategory(catArray, category)) {
      fprintf(stderr, "%s is not a valid category.\n", category);
      exit(EXIT_FAILURE);
    }
    const char * chosenWord = chooseWord(category, catArray);
    word = strdup(chosenWord);
    /* If reused is disabled, remove the word in the catArray. */
    if (reused == 0) {
      removeWord(catArray, category, chosenWord);
    }
  }

  free(category);
  return word;
}

/* initCategory() initializes the fields of the category_t struct
 * @return the initialized category_t struct
 */
category_t initCategory() {
  category_t result = {.name = NULL, .words = NULL, .n_words = 0};
  return result;
}

/* initCatarray() initialzes the fields of the catarray_t struct
 * @return a pointer to the initialized catarray_t struct
 */
catarray_t * initCatArray() {
  catarray_t * result = malloc(sizeof(*result));
  result->arr = NULL;
  result->n = 0;
  return result;
}

/* freeCategory frees the memory used in the category_t struct. */
void freeCategory(category_t category) {
  for (size_t i = 0; i < category.n_words; i++) {
    free(category.words[i]);
  }
  free(category.words);
  free(category.name);
}

/* freeCatArray frees the memory used in the catarray_t struct. */
void freeCatArray(catarray_t * catArray) {
  for (size_t i = 0; i < catArray->n; i++) {
    freeCategory(catArray->arr[i]);
  }
  free(catArray->arr);
  free(catArray);
}

/* getName extracts the characters before the colon,
 * and return as a string of name. 
 * @param nameStart, position(pointer) where the character of name starts
 * @param colon, the position(pointer) of the colon
 * @return the name string
 */
char * getName(char * nameStart, char * colon) {
  size_t nameLen = colon - nameStart;
  char * name = strndup(nameStart, nameLen);
  return name;
}

/* getWord extracts the characters after the colon,
 * and return as a string of word. 
 * @param wordStart, position(pointer) where the character of word starts
 * @param wordEnd, the position(pointer) where the character of word ends
 * @return the word string
 */
char * getWord(char * wordStart, char * wordEnd) {
  size_t wordLen = wordEnd - wordStart;
  char * nextLine = strchr(wordStart, '\n');
  /* Adjusting the copy length to exclude \n character. */
  if (nextLine != NULL) {
    wordLen -= 1;
  }
  char * word = strndup(wordStart, wordLen);
  return word;
}

/* addCategory adds a category with specified name and word to a catArray,
 * appending to an exist one, or add a new category node otherwise.
 * @param catArray, the catarray to be added
 * @param name, the name of category
 * @param word, the associated word
 */
void addCategory(catarray_t * catArray, char * name, char * word) {
  category_t * category = getCategory(catArray, name);
  if (category != NULL) {
    appendCategory(category, word);
  }
  else {
    addNewCategory(catArray, name, word);
  }
}

/* getCategory returns a category with the specified name in the catArray,
 * and returns NULL if the category does not exist.
 * @param catArray, the array of categories to be searched.
 * @param name, the taget name string.
 * @return the pointer to the category.
 */
category_t * getCategory(catarray_t * catArray, char * name) {
  for (size_t i = 0; i < catArray->n; i++) {
    if (strcmp(name, catArray->arr[i].name) == 0) {
      return &(catArray->arr[i]);
    }
  }
  return NULL;
}

/* appendCategory appends the word to the word array of the existing category.
 * @param category, the target category
 * @param word, the word string to be appended
 */
void appendCategory(category_t * category, const char * word) {
  category->words =
      realloc(category->words, (category->n_words + 1) * sizeof(*category->words));
  category->words[category->n_words] = strdup(word);
  category->n_words++;
}

/* addNewCategory initialize a new category with name and word,
 * and call appendCategory to the resized catArray.
 * @param catArray, the target catarray
 * @param name, the name string of the new category
 * @param word, the word string of the new category
 */
void addNewCategory(catarray_t * catArray, char * name, char * word) {
  catArray->arr = realloc(catArray->arr, (catArray->n + 1) * sizeof(*catArray->arr));
  catArray->arr[catArray->n] = initCategory();
  catArray->arr[catArray->n].name = strdup(name);
  appendCategory(&(catArray->arr[catArray->n]), word);
  catArray->n++;
}

/* removeWord removes a word in a category with specified name in a catArray.
 * @param catArray, the target catarray
 * @param name, the name of category
 * @param word, the associated word to be reomved
 */
void removeWord(catarray_t * catArray, char * name, const char * word) {
  category_t * category = getCategory(catArray, name);
  size_t idxOfDel = locateWord(category, word);
  /* If word does not exit then directly return. */
  if (idxOfDel == category->n_words) {
    return;
  }
  /* Free the the memory where the word string to be deleted stays. 
   * starting from the pointer where the word is deleted, 
   * move the pointer to point at the next one position,
   * and decrease the category->n_words to circumvent lately double free.
   * We belive this can approch the 'delete' functionality without any extra moves,
   * while keeping other words in order.
   */
  char * ptrToDel = category->words[idxOfDel];
  free(ptrToDel);
  for (size_t i = idxOfDel; i < category->n_words - 1; i++) {
    category->words[i] = category->words[i + 1];
  }
  category->n_words--;
}

/* locateWord returns the index of the target word in a category,
 * and will return the category->n_words (outrange index) 
 * if the words does not exits.
 * @param category, the category to be searched
 * @param word, the target word string
 * @return result, the index of the word in the category->words
 */
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

/* getCatArray resue the functions and codes in step 2,
 * return the generated catarray, reading from the target file.
 * Report any errors if it encounters then exits.
 */
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
  /* Iterate on the target file and read line by line. */
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

/* getSotry resue the functions and codes in step 1,
 * return the generated random story, reading from the target file.
 * Report any errors if it encounters then exits. 
 */
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

/* The final abtracted function tellStory, 
 * which combines all functionality in the previous steps
 * and have a -n option to indicate if resuing used words is allowed.
 * Report any errors if it encounters then exits.
 */
void tellStory(char * catWordFile, char * storyTemplate, int reused) {
  catarray_t * catArray = getCatArray(catWordFile);
  string_t * story = getStory(storyTemplate, catArray, reused);

  printf("%s", story->content);

  freeStringT(story);
  freeCatArray(catArray);
}
