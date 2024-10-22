#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define FILE_READER_BUFFER_SIZE 1000

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

void print_matrix_error(MatrixResultCode code) {
  switch (code) {
    case MATRIX_DIMENSIONS_MUST_BE_POSITIVE:
      printf("Validation failed: Matrix dimensions must be greater than 0.");
      break;
    case MATRIX_INDEX_ARGUMENT_OUT_OF_BOUNDS:
      printf("Error: Index out of bounds.");
      break;
    case MATRIX_ARGUMENTS_MUST_NOT_BE_NULL:
      printf("Error: Matrix should not be null.");
      break;
    case MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUM:
      printf("Validation failed: Matrices must have the same dimensions to calculate the sum.");
      break;
    case MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUBTRACT:
      printf("Validation failed: Matrices must have the same dimensions to calculate the subtract.");
      break;
    case MATRIX_INVALID_DIMENSIONS_TO_MULTIPLY:
      printf("Validation failed: Invalid dimensions for calculating multiplication.");
      break;
    case MATRIX_SHOULD_BE_SQUARE_TO_CALC_DETERMINANT:
      printf("Validation failed: Matrix should be square to calculate the determinant.");
      break;
    case MATRIX_INTERNAL_ERROR:
      printf("Error: Unexpected error occurred.");
      break;
    default:
      printf("Error: Unexpected error occurred.");
      break;
  }
}

int read_matrix_row(
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

      if (col > matrix->cols) {
        printf("Invalid file format: Invalid number of columns in matrix row %d. Expected: %d\n", row, matrix->cols);
        return 0;
      }
    } else if (isdigit(buffer[i]) || buffer[i] == '.' || buffer[i] == '-') {
      current_number[current_number_idx] = buffer[i];
      current_number_idx++;
    }
  }

  if (strlen(current_number) > 0) {
      current_number[current_number_idx] = '\0';
      double number = strtod(current_number, &unused_ptr);

      matrix_set(matrix, row + 1, col + 1, number);
  }

  return 1;
}

int read_matrix_from_file(Matrix *m, char* filename) {
  int buffer_length = FILE_READER_BUFFER_SIZE;
  char buffer[FILE_READER_BUFFER_SIZE];

  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    printf("Error: Failed to open file %s", filename);
    return 0;
  } 

  char *row_line = fgets(buffer, FILE_READER_BUFFER_SIZE, file);
  int rows = atoi(row_line);

  char *col_line = fgets(buffer, FILE_READER_BUFFER_SIZE, file);
  int cols = atoi(col_line);

  MatrixResult matrix_result = new_matrix(rows, cols);

  if (!matrix_result.success) {
    print_matrix_error(matrix_result.code);
    return 0;
  }
  
  Matrix *matrix = matrix_result.value;

  int row = 0;

  while (fgets(buffer, FILE_READER_BUFFER_SIZE, file) != NULL) {
    if (!read_matrix_row(buffer, matrix, row)) {
      printf("Error: Failed to parse matrix row %d of file %s", row, filename);
      return 0;
    };
    row++;
  }

  if (row != rows) {
    printf("Invalid file format: Matrix should have %d rows.", rows);
    return 0;
  }

  fclose(file);
  *m = *matrix;

  return 1;
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
        printf(", ");
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

int output_matrix_result(
  MatrixResult result,
  char *output_filename
) {
    if (result.success) {
      if (output_filename != NULL) {
        write_matrix_to_file(result.value, output_filename);
        printf("Success: Resulting matrix was saved to file '%s'.", output_filename);
      } else {
        print_matrix(result.value);
      }
    } else {
      print_matrix_error(result.code);
    }
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
  int read_a_file_result, read_b_file_result;
  MatrixNumericResult numeric_result;
  MatrixResult r;

  switch (operation_type) {
    case SUM:
      if (!read_matrix_from_file(&a, argv[2])) return;
      if (!read_matrix_from_file(&b, argv[3])) return;

      output_matrix_result(matrix_sum(&a, &b), argv[4]);

      break;
    case SUBTRACT:
      if (!read_matrix_from_file(&a, argv[2])) return;
      if (!read_matrix_from_file(&b, argv[3])) return;

      output_matrix_result(matrix_subtract(&a, &b), argv[4]);

      break;
    case MULTIPLY:
      if (!read_matrix_from_file(&a, argv[2])) return;
      if (!read_matrix_from_file(&b, argv[3])) return;
      
      output_matrix_result(matrix_multiply(&a, &b), argv[4]);

      break;
    case TRANSPOSE:
      if (!read_matrix_from_file(&a, argv[2])) return;

      output_matrix_result(matrix_transpose(&a), argv[3]);

      break;
    case DET:
      if (!read_matrix_from_file(&a, argv[2])) return;

      numeric_result = matrix_determinant(&a);

      if (numeric_result.success) {
        printf("det = %lf", numeric_result.value);
      } else {
        print_matrix_error(numeric_result.code);
      }

      break;
    default:
      break;
  }
}
