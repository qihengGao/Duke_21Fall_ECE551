#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

void run_check(int * array, size_t n, size_t expected_ans) {
  if (maxSeq(array, n) != expected_ans) {
    printf("A broken case found\n");
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  int a1[7] = {1, 1, 2, 4, 5, 7, 9};
  run_check(a1, 7, 6);
  int a2[1] = {9};
  run_check(a2, 1, 1);
  int a3[5] = {-1, 0, 1, 2, 3};
  run_check(a3, 5, 5);
  int a4[5] = {0};
  run_check(a4, 5, 0);
}
