#include "pandemic.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function prototypes.
 * Please see the detailed descriptions and implementations at the bottom. */
void parseName(char * line, char * comma, country_t * country_p);
void parsePopulation(char * comma, country_t * country_p);
unsigned getMaxCaseNum(unsigned * case_array, size_t size);

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
  char * comma = strchr(line, ',');
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
  /* NULL check. */
  if (data == NULL) {
    fprintf(stderr, "Input data is not exist.\n");
    exit(EXIT_FAILURE);
  }
  if (avg == NULL) {
    fprintf(stderr, "Input avg for recording is not exist.\n");
    exit(EXIT_FAILURE);
  }

  /* Check if the input is sufficient. */
  if (n_days < 7) {
    return;
  }

  /* Initialize an index indicator which indicates 
   * the next index of avg where the result should be stored*/
  size_t next_to_fill = 0;

  /* We carry out 'sliding window' approach, 
   * as we believe this approach can circumvent duplicated computation.  */
  /* The 'window' on the data array is characterized by the 'start' and 'end', 
   * and the window size is fixed as 7. 
   * curr_sum keeps track of  the sum of the elements in window.
   * when end = 6 (0-based), this is the first time our 'window' gets the size 7.
   * Then we can get the result by curr_sum / 7.0, put the result in the proper index.
   * increase next_to_fill by 1.
   * To keep the size fixed at 7, 
   * we 'squeeze the window' through increasing window start by 1,
   * 'making space' for window to include newly incoming elements.
   * Iterate through the data array unitl our window hits the end. */
  size_t curr_sum = 0;
  for (size_t start = 0, end = 0; end < n_days; end++) {
    curr_sum += data[end];
    if (end >= 6) {
      avg[next_to_fill] = curr_sum / 7.0;
      next_to_fill++;
      curr_sum -= data[start];
      start++;
    }
  }
}

void calcCumulative(unsigned * data, size_t n_days, uint64_t pop, double * cum) {
  /* NULL check. */
  if (data == NULL) {
    fprintf(stderr, "The input data is not exist.\n");
    exit(EXIT_FAILURE);
  }

  if (cum == NULL) {
    fprintf(stderr, "Input cum for recoding is not exist.\n");
    exit(EXIT_FAILURE);
  }

  /* Keep track of cummulative amout of cases. */
  size_t cumulative_sum = 0;

  /* Initialize an index indicator which indicates 
   * the next index of cum  where the result should be stored. */
  size_t next_to_fill = 0;

  /* Iterate on daily cases, calculating cumulative cases per 100,000 people. */
  for (size_t i = 0; i < n_days; i++) {
    cumulative_sum += data[i];
    cum[next_to_fill] = (cumulative_sum / (double)pop) * 100000;
    next_to_fill++;
  }
}

void printCountryWithMax(country_t * countries,
                         size_t n_countries,
                         unsigned ** data,
                         size_t n_days) {
  /* NULL check. */
  if (countries == NULL) {
    fprintf(stderr, "The input information of countries is not exist.\n");
    exit(EXIT_FAILURE);
  }

  if (data == NULL || *data == NULL) {
    fprintf(stderr, "The input cases data is not exist.\n");
    exit(EXIT_FAILURE);
  }

  unsigned max_case_num = 0;

  /* The coresponding country which has the maximum daily cases. */
  char * country_name = NULL;

  /* Keep track of the frequency of maximum in order to detect tie. */
  size_t frequency_of_max = 0;

  /* Iterate through the 2D array. */
  /* Obtain maximum case number for each country.
   * Update the corresponding country_name 
   * and max_case_num only when meet a greater case number,
   * and restore the frequency count to 1.
   * Or when meet a case number that is equal to current max_case_num,
   * increase the frequency to indicate a potential tie. */
  for (size_t i = 0; i < n_countries; i++) {
    unsigned max_cases_county_i = getMaxCaseNum(data[i], n_days);
    if (max_cases_county_i > max_case_num) {
      max_case_num = max_cases_county_i;
      country_name = countries[i].name;
      frequency_of_max = 1;
    }
    else if (max_cases_county_i == max_case_num) {
      frequency_of_max++;
    }
  }

  if (frequency_of_max > 1) { /* There exits a tie as the maximum is not unique. */
    printf("There is a tie between at least two countries\n");
  }
  else {
    printf("%s has the most daily cases with %u\n", country_name, max_case_num);
  }
}

/* Parse the name in line, print out error messages and exit if an error occurs. */
void parseName(char * line, char * comma, country_t * country_p) {
  const char * name_start;
  name_start = line;

  /* We can compute the country name length 
   * by taking the difference of pointer comma and pointer name_start.
   * As the name_start stores the address of the first character
   * and comma stores the address of the comma character,
   * their difference is the number of characters
   * from the name_start (inclusive) to the comma (exclusive).
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

/* Parse the population in line,
 * print out  print out error messages and exit if an error occurs. */
void parsePopulation(char * comma, country_t * country_p) {
  /* Initialize two pointers which both point at the one character after the comma. */
  char * population_start = comma + 1;
  char * population_end = comma + 1;

  /* We would argue that the population part 
   * is considered valid iff it contains digits only. */

  /* Parse string to integer. As the country.population is uint64_t, use strtoull. */
  unsigned long long int population = strtoull(population_start, &population_end, 10);
  if (population_end == population_start) {
    fprintf(stderr, "Population information is not included in input.\n");
    exit(EXIT_FAILURE);
  }

  /* Check overflow. */
  if (population == ULLONG_MAX && errno == ERANGE) {
    fprintf(stderr, "Input population is out of range.\n");
    exit(EXIT_FAILURE);
  }

  /* Assign the parsed integer to coutry_t.population. */
  country_p->population = (uint64_t)population;
}

/* Return the maximum value of the given array of cases. */
unsigned getMaxCaseNum(unsigned * case_array, size_t size) {
  if (case_array == NULL) {
    fprintf(stderr, "Input case array is empty.\n");
    exit(EXIT_FAILURE);
  }

  unsigned maximum = 0;
  for (size_t i = 0; i < size; i++) {
    if (case_array[i] > maximum) {
      maximum = case_array[i];
    }
  }

  return maximum;
}
