#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned base, unsigned exponent) {
  if (exponent == 0)
    return 1;
  return base * power(base, exponent - 1);
}
