#include "kv.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvpair_t * createNode(char * buffer, ssize_t read_len);
void addNode(char * buffer, ssize_t read_len, kvarray_t * result);

kvarray_t * readKVs(const char * fname) {
  FILE * f = fopen(fname, "r");
  if (!f) {
    perror("Cannot open file.");
    exit(EXIT_FAILURE);
  }
  // initialize result to return
  kvarray_t * result = malloc(sizeof(*result));
  result->array = malloc(sizeof(*result->array));
  result->length = 0;

  // read file line by line
  char * key_val_buffer = NULL;
  size_t sz = 0;
  ssize_t read_len = 0;
  while ((read_len = getline(&key_val_buffer, &sz, f)) != -1) {
    addNode(key_val_buffer, read_len, result);
  }
  free(key_val_buffer);
  if (fclose(f) != 0) {
    perror("Cannot close file.\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

void freeKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->length; i++) {
    kvpair_t * node = pairs->array[i];
    free(node->key);
    free(node->val);
    free(node);
  }
  free(pairs->array);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  for (size_t i = 0; i < pairs->length; i++) {
    kvpair_t * node = pairs->array[i];
    char * key = node->key;
    char * val = node->val;
    printf("key = '%s' value = '%s'\n", key, val);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  for (size_t i = 0; i < pairs->length; i++) {
    if (strcmp(pairs->array[i]->key, key) == 0) {
      return pairs->array[i]->val;
    }
  }
  return NULL;
}

void addNode(char * buffer, ssize_t read_len, kvarray_t * result) {
  kvpair_t * node = createNode(buffer, read_len);
  if (result->length == 0) {
    result->array[result->length++] = node;
    return;
  }
  if (result->array[result->length - 1] != NULL) {
    result->array = realloc(result->array, 2 * result->length * sizeof(*result->array));
    assert(result->array != NULL);
  }
  result->array[result->length++] = node;
}

kvpair_t * createNode(char * buffer, ssize_t read_len) {
  char * sep = strchr(buffer, '=');
  if (sep == NULL) {
    fprintf(stderr, "Separator '=' does not exits. \n");
    exit(EXIT_FAILURE);
  }
  size_t key_len = sep - buffer;
  size_t val_len = (buffer[read_len - 1] == '\n') ? ((size_t)read_len - 1 - key_len - 1)
                                                  : (size_t)read_len - key_len - 1;
  kvpair_t * node = malloc(sizeof(*node));

  node->key = malloc((key_len + 1) * sizeof(*node->key));
  node->val = malloc((val_len + 1) * sizeof(*node->val));
  memcpy(node->key, buffer, key_len);
  memcpy(node->val, sep + 1, val_len);
  node->key[key_len] = '\0';
  node->val[val_len] = '\0';

  return node;
}
