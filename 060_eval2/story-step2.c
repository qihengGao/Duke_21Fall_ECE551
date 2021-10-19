#include <stdio.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./story-step2 input_file\n");
    exit(EXIT_FAILURE);
  }
  char * fileName = argv[1];
  FILE * f = fopen(fileName, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot oppen file: %s", fileName);
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
  printWords(catArray);
  freeCatArray(catArray);
  if (fclose(f) == EOF) {
    fprintf(stderr, "Cannot close file : %s.\n", fileName);
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
