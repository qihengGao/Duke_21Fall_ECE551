#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 10

// Form matrix from input file. Return EXIT_SUCESS if matrix is successfully formed, otherwise return EXIT_FAILURE.
int getMatrix(int matrix[][SIZE], FILE * f) {
  char line[SIZE + 2];  // extra one space for '\n' and terminator
  for (int i = 0; i < SIZE; i++) {
    if (!fgets(line, SIZE + 2, f)) {
      fprintf(stderr, "Inputfile has lines fewer than 10.\n");
      return EXIT_FAILURE;
    }
    if (!strchr(line, '\n')) {
      fprintf(stderr, "Length of intput in line %d is greater than 10.\n", i);
      return EXIT_FAILURE;
    }
    for (int j = 0; j < SIZE; j++) {
      if (line[j] == '\0') {
        fprintf(stderr, "Length of input in line %d is smaller than 10.\n", i);
        return EXIT_FAILURE;
      }
      matrix[i][j] = line[j];
    }
  }
  if (fgets(line, SIZE + 2, f)) {
    fprintf(stderr, "Inputfile has more than 10 lines.\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void transposeMatrix(int matrix[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = i + 1; j < SIZE; j++) {
      int temp = matrix[i][j];
      matrix[i][j] = matrix[j][i];
      matrix[j][i] = temp;
    }
  }
}

void reflexMatrix(int matrix[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    int left = 0, right = SIZE - 1;
    while (left < right) {
      int temp = matrix[i][left];
      matrix[i][left] = matrix[i][right];
      matrix[i][right] = temp;
      left++;
      right--;
    }
  }
}

void rotateMatrix(int matrix[][SIZE]) {
  transposeMatrix(matrix);
  reflexMatrix(matrix);
}

void printMatrix(int matrix[][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: roateMatrix inputFile.\n");
    return EXIT_FAILURE;
  }
  char * fileName = argv[1];
  FILE * f = fopen(fileName, "r");
  if (!f) {
    fprintf(stderr, "File: %s is not found.\n", fileName);
    return EXIT_FAILURE;
  }
  int matrix[SIZE][SIZE] = {{0}};
  if (getMatrix(matrix, f) != EXIT_SUCCESS) {
    fprintf(stderr, "Program ends as it encounters trouble in forming matrix.\n");
    return EXIT_FAILURE;
  }
  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close %s.\n", fileName);
    return EXIT_FAILURE;
  }
  rotateMatrix(matrix);
  printMatrix(matrix);
  return EXIT_FAILURE;
}
