#include <stdio.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: ./story-step3 category_words.txt story.txt\n");
    exit(EXIT_FAILURE);
  }

  char * catWordFile = argv[1];
  catarray_t * catArray = getCatArray(catWordFile);

  char * storyTemplate = argv[2];
  string_t * story = getStory(storyTemplate, catArray);

  printf("%s", story->content);

  freeStringT(story);
  freeCatArray(catArray);

  return EXIT_SUCCESS;
}
