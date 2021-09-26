#include "pandemic.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes. Please see the detailed descriptions and implementations at the bottom. */
void parseName(const char * line, const char * comma, country_t * country_p);
void parsePopulation(const char * comma, country_t * country_p);

country_t parseLine(char * line) {
  country_t country;
  country.name[0] = '\0';
  country.population = 0;

  /* Sanity check: input line has contents.*/
  if (line == NULL || strlen(line) == 0) {
    fprintf(stderr, "Empty Input.\n");
    exit(EXIT_FAILURE);
  }
  /* Check if the comma exists. */
  const char * comma = strchr(line, ',');
  if (comma == NULL) {
    fprintf(
        stderr,
        "The input line does not match the standard format: a comma does not exist.\n");
    exit(EXIT_FAILURE);
  }
  /*  We consider characters before the first comma as the name of the country. */
  parseName(line, comma, &country);
  /* The digits after the first comma as the population. */
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

/* Parse the name in line, print out error messages and exit if an error occurs. */
void parseName(const char * line, const char * comma, country_t * country_p) {
  const char * name_start;
  name_start = line;

  /* We can compute the country name length by taking the difference of pointer comma and pointer name_start.
   * As the name_start stores the address of the first character and comma stores the address of the comma character,
   * their difference is the number of characters from the name_start (inclusive) to the comma (exclusive).
   */
  size_t name_len = (size_t)(comma - name_start);

  /* Check if the name length is too long */
  if (name_len > MAX_NAME_LEN - 1) {
    fprintf(stderr, "The name is too long. (length <= 63)\n");
    exit(EXIT_FAILURE);
  }

  /* Copy the characters to the country_t's name field. */
  memcpy(country_p->name, name_start, name_len);
  /* Set terminator properly. */
  country_p->name[name_len] = '\0';
}

/* Parse the population in line, print out  print out error messages and exit if an error occurs. */
void parsePopulation(const char * comma, country_t * country_p) {
  /* Initialize two pointers which both point at the one character after the comma. */
  const char * population_start;
  const char * population_end;
  population_start = comma + 1;
  population_end = comma + 1;

  /* Iterate on the rest of line to check if it only contains digits. */
  /* Set the end character where the iteration should end, generally it is '\n', but if it is not exist,
   * then the line ends without a '\n', so we set the end character to '\0'. 
   */
  const char * end_p = strchr(population_start, '\n');
  char end = (end_p == NULL) ? '\0' : *end_p;
  /* Iteration starts. */
  while (*population_end != end) {
    /* Non-digit character occurs, exit. */
    if (!isdigit(*population_end)) {
      fprintf(stderr,
              "Unexpected character: '%c' appears in the population information, which "
              "should contain digits only.\n",
              *population_end);
      exit(EXIT_FAILURE);
    }
    population_end++;
  }

  /* Similar computation as getting the name length in parseName. */
  size_t population_len = (size_t)(population_end - population_start);
  if (population_len == 0) {
    fprintf(stderr, "Population information is not included in input.\n");
    exit(EXIT_FAILURE);
  }

  /* Parse string to integer. As the country.population is uint64_t, use strtoul. */
  unsigned long int population = strtoul(population_start, NULL, 10);
  /* Check overflow. */
  if (population == ULONG_MAX && errno == ERANGE) {
    fprintf(stderr, "Input population is out of range.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the parsed integer to coutry.population. */
  country_p->population = (uint64_t)population;
}
