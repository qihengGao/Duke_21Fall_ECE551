#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>

#include "provided.h"
/* Structs and functions that are used in this project. 
 * Please check rand_story.c for detatiled implementations and
 * documentation.
 */

/* Step 1*/
/* struct for storing
 * dynamically-sized string (char * content)
 * and its length (size_t).
 */
struct string_tag {
  char * content;
  size_t len;
};
typedef struct string_tag string_t;

string_t * initStringT();
void freeStringT(string_t * str);

void appendChars(string_t * dest, const char * src, size_t srcLen);
void appendStringT(string_t * dest, string_t * src);

int checkBlank(char * line, ssize_t readLen);
string_t * parseLine(char * line, ssize_t readLen, catarray_t * catArray);

/* Step 2 */
category_t initCategory();
catarray_t * initCatArray();
void freeCategory(category_t category);
void freeCatArray(catarray_t * catArray);

char * getName(char * nameStart, char * colon);
char * getWord(char * wordStart, char * wordEnd);
void addCategory(catarray_t * catArray, char * name, char * word);
category_t * getCategory(catarray_t * catArray, char * name);
void appendCategory(category_t * category, char * word);
void addNewCategory(catarray_t * catArray, char * name, char * word);

/* Step 3 */
catarray_t * getCatArray(char * catWordFile);
string_t * getStory(char * storyTemplate, catarray_t * catArray);
#endif
