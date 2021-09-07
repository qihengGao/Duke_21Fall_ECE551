#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned int x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected) {
  if (expected != power(x, y)) {
    printf("A broken case foud.");
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  run_check(23, 0, 1);
  run_check(1, 100, 1);
  run_check(3, 3, 27);
  run_check(0, 0, 1);
  run_check(-2, 3, -8);
  run_check(0, 10, 0);
  //  printf("All good!\n");
}
