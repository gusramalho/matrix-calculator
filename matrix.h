#ifndef MATRIX_H
#define MATRIX_H

typedef struct Matrix {
    int rows;
    int cols;
    double **values;
} Matrix;

Matrix* new_matrix(int rows, int cols);

Matrix* new_matrix_with_values(int rows, int cols, double values[rows][cols]);

Matrix* new_identity_matrix(int n);

void matrix_set(Matrix *m, int row, int col, double value);

double matrix_get(Matrix *m, int row, int col);

Matrix* matrix_sum(Matrix *a, Matrix *b);

Matrix* matrix_subtract(Matrix *a, Matrix *b);

Matrix* matrix_multiply_n(Matrix *a, int n);

Matrix* matrix_multiply(Matrix *a, Matrix *b);

double matrix_determinant(Matrix *m);

Matrix* matrix_transpose(Matrix *a);

int matrix_equals(Matrix *a, Matrix *b);

void delete_matrix(Matrix *m);

#endif // MATRIX_H

