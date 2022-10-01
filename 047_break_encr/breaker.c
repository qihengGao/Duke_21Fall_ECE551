#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void find_key(FILE * f);
void count_freq(int * charToFreq, FILE * f);
int get_max_index(int * charToFreq);

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: broker inputFileName.");
    return EXIT_FAILURE;
  }
  const char * fileName = argv[1];
  FILE * f = fopen(fileName, "r");
  if (!f) {
    perror("Cannot open the file.");
    return EXIT_FAILURE;
  }
  find_key(f);
  if (!fclose(f)) {
    perror("Failed to close the file.");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void find_key(FILE * f) {
  int charToFreq[26] = {0};
  // get the frequency of each character in the file.
  count_freq(charToFreq, f);
  // idx = the relative position to char 'a'
  int maxFreqIdx = get_max_index(charToFreq);
  /* if (maxFreqIdx >= 4) { */
  /*   printf("%d\n", maxFreqIdx - 4); */
  /*   return; */
  /* } */
  /* // ('e' + key) % 26  - 'a' = index; */
  /* // key % 26 + ('e' - 'a') = index; */
  /* // key % 26 = index - 4; */
  /* // key = index - 4 + 26; // +22 */
  /* printf("%d\n", maxFreqIdx + 22); */
  int offset = 0;
  for (; offset < 26; ++offset) {
    if ((4 + offset) % 26 == maxFreqIdx) {
      printf("%d\n", offset);
    }
  }
  printf("%d\n", 0);
}

void count_freq(int * charToFreq, FILE * f) {
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      charToFreq[c - 'a']++;
    }
  }
}

int get_max_index(int * charToFreq) {
  int result = 0;
  for (int i = 1; i < 26; i++) {
    if (charToFreq[i] > charToFreq[result]) {
      result = i;
    }
  }
  return result;
}
