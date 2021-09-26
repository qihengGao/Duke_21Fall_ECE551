#include "pandemic.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseName(const char * line, const char * comma, country_t * country_p);
void parsePopulation(const char * comma, country_t * country_p);

country_t parseLine(char * line) {
  country_t country;
  country.name[0] = '\0';
  country.population = 0;

  /** Sanity check: input line has contents.*/
  if (line == NULL || strlen(line) == 0) {
    fprintf(stderr, "Empty Input.\n");
    exit(EXIT_FAILURE);
  }

  const char * comma = strchr(line, ',');
  if (comma == NULL) {
    fprintf(
        stderr,
        "The input line does not match the standard format: a comma does not exist.\n");
    exit(EXIT_FAILURE);
  }

  parseName(line, comma, &country);
  parsePopulation(comma, &country);

  return country;
}

void calcRunningAvg(unsigned * data, size_t n_days, double * avg) {
  //WRITE ME
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  //WRITE ME
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  //WRITE ME
}

void parseName(const char * line, const char * comma, country_t * country_p) {
  const char * name_start;
  const char * name_end;
  name_start = line;
  name_end = line;
  while (name_end != comma) {
    name_end++;
  }
  size_t name_len = (size_t)(name_end - name_start);
  if (name_len > MAX_NAME_LEN - 1) {
    fprintf(stderr, "The name is too long.\n");
    exit(EXIT_FAILURE);
  }
  memcpy(country_p->name, name_start, name_len);
  country_p->name[name_len] = '\0';
}

void parsePopulation(const char * comma, country_t * country_p) {
  const char * population_start;
  const char * population_end;
  population_start = comma + 1;
  population_end = comma + 1;
  const char * end_p = strchr(population_start, '\n');
  char end = (end_p == NULL) ? '\0' : *end_p;
  while (*population_end != end) {
    if (!isdigit(*population_end)) {
      fprintf(stderr,
              "Unexpected character: '%c' appears in the population information, which "
              "should contain digits only.\n",
              *population_end);
      exit(EXIT_FAILURE);
    }
    population_end++;
  }
  size_t population_len = (size_t)(population_end - population_start);
  if (population_len == 0) {
    fprintf(stderr, "Population information is not included in input.\n");
    exit(EXIT_FAILURE);
  }
  unsigned long int population = strtoul(population_start, NULL, 10);
  if (population == ULONG_MAX && errno == ERANGE) {
    fprintf(stderr, "Input population is out of range.\n");
    exit(EXIT_FAILURE);
  }
  country_p->population = (uint64_t)population;
}
