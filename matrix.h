#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix {
    int rows;
    int cols;
    double **values;
} Matrix;

typedef enum MatrixResultCode {
    MATRIX_SUCCESS_CODE,
    MATRIX_DIMENSIONS_MUST_BE_POSITIVE,
    MATRIX_INDEX_ARGUMENT_OUT_OF_BOUNDS,
    MATRIX_ARGUMENTS_MUST_NOT_BE_NULL,
    MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUM,
    MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUBTRACT,
    MATRIX_INVALID_DIMENSIONS_TO_MULTIPLY,
    MATRIX_SHOULD_BE_SQUARE_TO_CALC_DETERMINANT,
    MATRIX_INTERNAL_ERROR
} MatrixResultCode;

typedef struct MatrixResult {
    int success;
    MatrixResultCode code;
    Matrix *value;
} MatrixResult;

typedef struct MatrixNumericResult {
    int success;
    MatrixResultCode code;
    double value;
} MatrixNumericResult;

MatrixResult new_matrix(int rows, int cols);

MatrixResult new_matrix_with_values(int rows, int cols, double values[rows][cols]);

Matrix* matrix_copy(Matrix *m);

MatrixResult new_identity_matrix(int n);

MatrixNumericResult matrix_set(Matrix *m, int row, int col, double value);

MatrixNumericResult matrix_get(Matrix *m, int row, int col);

MatrixResult matrix_sum(Matrix *a, Matrix *b);

MatrixResult matrix_subtract(Matrix *a, Matrix *b);

MatrixResult matrix_multiply(Matrix *a, Matrix *b);

MatrixNumericResult matrix_determinant_laplace(Matrix *m);

MatrixNumericResult matrix_determinant_lu_decomposition(Matrix *m);

MatrixResult matrix_transpose(Matrix *m);

int matrix_equals(Matrix *a, Matrix *b);

void delete_matrix(Matrix *m);

#endif // MATRIX_H

