#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void sortLines(FILE * f) {
  char ** lines = NULL;
  char * curr = NULL;
  size_t size;
  size_t count = 0;
  while (getline(&curr, &size, f) != -1) {
    lines = realloc(lines, (count + 1) * sizeof(*lines));
    lines[count] = curr;
    curr = NULL;
    count++;
  }
  free(curr);
  sortData(lines, count);
  for (size_t i = 0; i < count; i++) {
    printf("%s", lines[i]);
    free(lines[i]);
  }
  free(lines);
}

void sortLinesInFiles(int argc, char ** argv) {
  for (int i = 0; i < argc - 1; i++) {
    FILE * f = fopen(argv[i + 1], "r");
    if (!f) {
      fprintf(stderr, "Cannot open file.\n");
      exit(EXIT_FAILURE);
    }
    sortLines(f);
    if (fclose(f) != 0) {
      fprintf(stderr, "Cannot close the file.\n");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc == 1) {
    sortLines(stdin);
  }
  else if (argc > 1) {
    sortLinesInFiles(argc, argv);
  }
  return EXIT_SUCCESS;
}
