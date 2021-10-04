#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUFFIX_LEN 8  // .counts\0
char * computeOutputFileName(const char * inputName) {
  size_t name_len = strlen(inputName);
  char * output_name = malloc((name_len + SUFFIX_LEN) * sizeof(*output_name));
  memcpy(output_name, inputName, name_len);
  memcpy(output_name + name_len, ".counts", SUFFIX_LEN);
  return output_name;
}
