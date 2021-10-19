#include <stdio.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  /* Command-line argument number check. */
  if (argc != 2) {
    fprintf(stderr, "Usage: ./story-step1 input_file \n");
    exit(EXIT_FAILURE);
  }

  char * fileName = argv[1];
  FILE * f = fopen(fileName, "r");
  if (f == NULL) {
    fprintf(stderr, "File: %s does not exist.\n", fileName);
    exit(EXIT_FAILURE);
  }

  string_t * parsedStory = initStringT();

  char * line = NULL;
  size_t bufferSize = 0;
  ssize_t readLen = 0;
  while ((readLen = getline(&line, &bufferSize, f)) != -1) {
    if (checkBlank(line, readLen) == 0) {
      fprintf(stderr,
              "The blank does not have matching closed underscore in the same line.\n");
      exit(EXIT_FAILURE);
    }
    string_t * prunedLine = parseLine(line, readLen, NULL);
    appendStringT(parsedStory, prunedLine);
    freeStringT(prunedLine);
  }
  free(line);

  printf("%s", parsedStory->content);

  freeStringT(parsedStory);
  if (fclose(f) == EOF) {
    fprintf(stderr, "Cannot close file: %s", fileName);
  }
  return EXIT_SUCCESS;
}
