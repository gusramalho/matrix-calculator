#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include <stdlib.h>
#include <ctype.h>

#define MATRIX_SIZE 15

void write_matrix_to_file(Matrix *m, char *filename) {
  FILE *file = fopen(filename, "w");

  fprintf(file, "%d\n", m->rows);
  fprintf(file, "%d\n", m->cols);

  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      fprintf(file, "%.2lf", matrix_get(m, i + 1, j + 1).value);

      if (j < m->cols - 1) {
        fprintf(file, ",");
      }
    }
    fprintf(file, "\n");
  }

  fclose(file);
}

int random_int() {
  int i = rand();
  return (rand() % 30) * (i % 2 == 0 ? 1 : -1);
}

void main(int argc, char* argv[]) {
  int size = atoi(argv[1]);

  int rows = size;
  int cols = size;

  Matrix *m = new_matrix(rows, cols).value;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix_set(m, i + 1, j + 1, random_int());
    }
  }

  char filename[50] = "";

  sprintf(filename, "test-matrix-%dx%d.mat", size);

  write_matrix_to_file(m, filename);
}