#include <stdio.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Usage: ./story-step4 (-n) category_words.txt story.txt\n");
    exit(EXIT_FAILURE);
  }

  int reused = 1;
  if (argc == 4) {
    if (strcmp(argv[1], "-n") != 0) {
      fprintf(stderr, "Option: -n is not in the expected position\n");
      fprintf(stderr, "Usage: ./story-step4 -n category_words.txt story.txt\n");
      exit(EXIT_FAILURE);
    }
    reused = 0;
  }

  char * catWordFile = (argc == 3) ? argv[1] : argv[2];
  char * storyTemplate = (argc == 3) ? argv[2] : argv[3];
  tellStory(catWordFile, storyTemplate, reused);

  return EXIT_SUCCESS;
}
