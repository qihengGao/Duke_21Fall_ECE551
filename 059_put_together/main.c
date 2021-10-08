#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  if (filename == NULL || kvPairs == NULL)
    return NULL;
  FILE * f = fopen(filename, "r");
  if (!f) {
    perror("Cannot open file.\n");
    exit(EXIT_FAILURE);
  }
  counts_t * c = createCounts();
  char * line = NULL;
  size_t size = 0;
  ssize_t len = 0;
  while ((len = getline(&line, &size, f)) != -1) {
    char * key = strndup(line, len - 1);
    char * val = lookupValue(kvPairs, key);
    free(key);
    addCount(c, val);
  }
  free(line);
  if (fclose(f) == EOF) {
    perror("Cannot close file. \n");
    exit(EXIT_FAILURE);
  }
  return c;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if (argc < 2) {
    fprintf(stderr, "Please check input number.\n");
    exit(EXIT_FAILURE);
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * pairs = readKVs(argv[1]);
  //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    char * inputName = argv[i];
    counts_t * c = countFile(inputName, pairs);
    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(inputName);
    //open the file named by outName (call that f)
    FILE * outFile = fopen(outName, "w");
    //print the counts from c into the FILE f
    printCounts(c, outFile);
    //close f
    if (fclose(outFile) == EOF) {
      perror("Cannot close outFile.\n");
      exit(EXIT_FAILURE);
    }
    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }
  //free the memory for kv
  freeKVs(pairs);
  return EXIT_SUCCESS;
}
