#include "counts.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

one_count_t * createOneNode(const char * name, size_t count);
void addNewNode(counts_t * c, const char * name);

counts_t * createCounts(void) {
  counts_t * counts = malloc(sizeof(*counts));
  counts->nodes = NULL;
  counts->size = 0;
  counts->next = 0;
  counts->unknown = 0;
  return counts;
}
void addCount(counts_t * c, const char * name) {
  if (name == NULL) {
    c->unknown++;
    return;
  }
  for (size_t i = 0; i < c->next; i++) {
    if (strcmp((c->nodes[i])->val, name) == 0) {
      (c->nodes[i])->count++;
      return;
    }
  }
  addNewNode(c, name);
}
void printCounts(counts_t * c, FILE * outFile) {
  if (c->next == 0)
    return;
  // zu for size_t
  for (size_t i = 0; i < c->next; i++) {
    fprintf(outFile, "%s: %zu\n", c->nodes[i]->val, c->nodes[i]->count);
  }
  if (c->unknown > 0)
    fprintf(outFile, "<unknown> : %zu\n", c->unknown);
}

void freeCounts(counts_t * c) {
  for (size_t i = 0; i < c->next; i++) {
    free(c->nodes[i]->val);
    free(c->nodes[i]);
  }
  free(c->nodes);
  free(c);
}

void addNewNode(counts_t * c, const char * name) {
  if (c->next == c->size) {
    if (c->size == 0) {
      c->size = 1;
    }
    c->nodes = realloc(c->nodes, 2 * c->size * sizeof(*c->nodes));
    assert(c->nodes);
    c->size *= 2;
  }
  one_count_t * node = createOneNode(name, 1);
  assert(c->next < c->size);
  c->nodes[c->next] = node;
  c->next++;
}

one_count_t * createOneNode(const char * name, size_t count) {
  one_count_t * node = malloc(sizeof(*node));
  node->val = malloc((strlen(name) + 1) * sizeof(*node->val));
  node->count = count;
  memcpy(node->val, name, strlen(name));
  node->val[strlen(name)] = '\0';
  return node;
}
