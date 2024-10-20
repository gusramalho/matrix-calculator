#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include <stdlib.h>
#include <ctype.h>

#define FILE_READER_BUFFER_SIZE 255

typedef enum MatrixOperationType {
  SUM,
  SUBTRACT,
  MULTIPLY,
  DET,
  TRANSPOSE,
  INVALID_OPERATION
} MatrixOperationType;

typedef struct MatrixOperation {
  Matrix *a;
  Matrix *b;
  MatrixOperationType operator;
} MatrixOperation;

char OPERATIONS[5][20] = {
  "--sum",
  "--subtract",
  "--multiply",
  "--det",
  "--transpose"
};

void read_matrix_row(
  char* buffer,
  Matrix *matrix,
  int row
) {
  char *unused_ptr;
  int col = 0;
  char current_number[25];
  int current_number_idx = 0;

  for (int i = 0; i < strlen(buffer); i++) {
    if (buffer[i] == ',' || buffer[i] == '\n' || buffer[i] == EOF) {
      current_number[current_number_idx] = '\0';
      double number = strtod(current_number, &unused_ptr);
      matrix_set(matrix, row + 1, col + 1, number);
      strcpy(current_number, "");
      current_number_idx = 0;
      col++;
    } else if (isdigit(buffer[i]) || buffer[i] == '.') {
      current_number[current_number_idx] = buffer[i];
      current_number_idx++;
    }
  }

  if (strlen(current_number) > 0) {
      current_number[current_number_idx] = '\0';
      double number = strtod(current_number, &unused_ptr);

      matrix_set(matrix, row + 1, col + 1, number);
  }
}

int read_matrix_from_file(Matrix *m, char* filename) {
  int buffer_length = FILE_READER_BUFFER_SIZE;
  char buffer[FILE_READER_BUFFER_SIZE];

  FILE *file = fopen(filename, "r");

  if (file == NULL) return -1;

  char *row_line = fgets(buffer, FILE_READER_BUFFER_SIZE, file);
  int rows = atoi(row_line);

  char *col_line = fgets(buffer, FILE_READER_BUFFER_SIZE, file);
  int cols = atoi(col_line);

  MatrixResult matrix_result = new_matrix(rows, cols);

  if (!matrix_result.success) {
    return -1;
  }
  
  Matrix *matrix = matrix_result.value;

  int row = 0;

  while (fgets(buffer, FILE_READER_BUFFER_SIZE, file) != NULL) {
    read_matrix_row(buffer, matrix, row);
    row++;
  }

  fclose(file);
  *m = *matrix;

  return 0;
}

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

void print_matrix(Matrix *m) {
  printf("\n");

  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("%.2lf", matrix_get(m, i + 1, j + 1).value);

      if (j < m->cols - 1) {
        printf(",");
      }
    }
    printf("\n");
  }
}

MatrixOperationType parse_operation_type(char* operation) {
  for (int i = 0; i < 5; i++) {
    if (strcmp(OPERATIONS[i], operation) == 0) {
      return i;
    }
  }

  return INVALID_OPERATION;
}

void main(int argc, char* argv[]) {
  if (argv[1] == NULL) {
    printf("No operation was requested.");
    return;
  }

  MatrixOperationType operation_type = parse_operation_type(argv[1]);

  if (operation_type == INVALID_OPERATION) {
    printf("Operation '%s' is invalid.", argv[1]);
    return;
  }

  Matrix a, b;
  MatrixNumericResult numeric_result;
  MatrixResult r;

  switch (operation_type) {
    case SUM:
      read_matrix_from_file(&a, argv[2]);
      read_matrix_from_file(&b, argv[3]);
      r = matrix_sum(&a, &b);
      write_matrix_to_file(r.value, argv[4]);

      break;
    case SUBTRACT:
      read_matrix_from_file(&a, argv[2]);
      read_matrix_from_file(&b, argv[3]);
      r = matrix_subtract(&a, &b);
      write_matrix_to_file(r.value, argv[4]);

      break;
    case MULTIPLY:
      read_matrix_from_file(&a, argv[2]);
      read_matrix_from_file(&b, argv[3]);
      r = matrix_multiply(&a, &b);
      write_matrix_to_file(r.value, argv[4]);

      break;
    case TRANSPOSE:
      read_matrix_from_file(&a, argv[2]);
      r = matrix_transpose(&a);
      write_matrix_to_file(r.value, argv[3]);

      break;
    case DET:
      read_matrix_from_file(&a, argv[2]);
      numeric_result = matrix_determinant(&a);
      printf("det = %lf", numeric_result.value);

      break;
  default:
    break;
  }
}
