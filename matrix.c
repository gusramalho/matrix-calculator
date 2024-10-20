#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

Matrix* new_matrix(int rows, int cols) {
    Matrix *m = (Matrix*) malloc(sizeof(Matrix));

    m->rows = rows;
    m->cols = cols;
    m->values = (double**) malloc(rows * sizeof(double*));

    for (int i = 0; i < rows; i++) {
        m->values[i] = (double*) malloc(cols * sizeof(double));

        for (int j = 0; j < cols; j++) {
            m->values[i][j] = 0;
        }
    }

    return m;
}

double _get(Matrix *m, int row, int col) {
    return m->values[row][col];
}

void _set(Matrix *m, int row, int col, double value) {
    m->values[row][col] = value;
}

Matrix* new_matrix_with_values(int rows, int cols, double values[rows][cols]) {
    Matrix *m = new_matrix(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            _set(m, i, j, values[i][j]);
        }
    }

    return m;
}

Matrix* new_identity_matrix(int n) {
    Matrix *m = new_matrix(n, n);

    for (int i = 0; i < n; i++) {
        _set(m, i, i, 1);
    }

    return m;
}

Matrix* matrix_transpose(Matrix *a) {
    Matrix *result = new_matrix(a->cols, a->rows);

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            _set(result, j, i, _get(a, i, j));
        }
    }

    return result;
}

double matrix_get(Matrix *m, int row, int col) {
    return m->values[row][col];
}

void matrix_set(Matrix *m, int row, int col, double value) {
    if (row >= 0 && row < m->rows && col >= 0 && col < m->cols) {
        m->values[row][col] = value;
    }
}

int matrix_equals(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return 0;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            if (_get(a, i, j) != _get(b, i, j)) {
                return 0;
            }
        }
    }

    return 1;
}

Matrix* matrix_sum(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return NULL;

    Matrix *result = new_matrix(a->rows, a->cols);

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            _set(result, i, j, _get(a, i, j) + _get(b, i, j));
        }
    }

    return result;
}

Matrix* matrix_subtract(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return NULL;

    Matrix *result = new_matrix(a->rows, a->cols);

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            _set(result, i, j, _get(a, i, j) - _get(b, i, j));
        }
    }

    return result;
}

float _multiply_row_by_col(Matrix *a, int a_row, Matrix *b, int b_col) {
    int size = a->cols;
    double result = 0;

    for (int i = 0; i < size; i++) {
        result += _get(a, a_row, i) * _get(b, i, b_col);
    }

    return result;
}

Matrix* matrix_multiply(Matrix *a, Matrix *b) {
    if (a->cols != b->rows) return NULL;

    Matrix *result = new_matrix(a->rows, b->cols);

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            _set(result, i, j, _multiply_row_by_col(a, i, b, j));
        }
    }

    return result;
}

double _determinant_2x2(Matrix *m) {
    return _get(m, 0, 0) * _get(m, 1, 1) - (
        _get(m, 0, 1) * _get(m, 1, 0)
    );
}

double _determinant_3x3(Matrix *m) {
    double diagonal_1 = _get(m, 0, 0) * _get(m, 1, 1) * _get(m, 2, 2);
    double diagonal_2 = _get(m, 0, 1) * _get(m, 1, 2) * _get(m, 2, 0);
    double diagonal_3 = _get(m, 0, 2) * _get(m, 1, 0) * _get(m, 2, 1);
    double diagonal_4 = _get(m, 0, 2) * _get(m, 1, 1) * _get(m, 2, 0);
    double diagonal_5 = _get(m, 0, 0) * _get(m, 1, 2) * _get(m, 2, 1);
    double diagonal_6 = _get(m, 2, 2) * _get(m, 1, 0) * _get(m, 0, 1);

    return diagonal_1 + diagonal_2 + diagonal_3 - (
        diagonal_4 + diagonal_5 + diagonal_6
    );
}

Matrix* _new_complementary_matrix(Matrix *m, int row_to_remove, int col_to_remove) {
    Matrix *result = new_matrix(m->rows - 1, m->cols - 1);

    int row_to_skip = 0;
    int col_to_skip = 0;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            if (i == row_to_remove) {
                row_to_skip = 1;
            }

            if (j == col_to_remove) {
                col_to_skip = 1;
            }

            if (i != row_to_remove && j != col_to_remove) {
                _set(result, i - row_to_skip, j - col_to_skip, _get(m, i, j));
            }
        }
        col_to_skip = 0;
    }

    return result;
}


double matrix_determinant(Matrix *m) {
    if (m->rows == 2 && m->cols == 2) {
        return _determinant_2x2(m);
    }

    if (m->rows == 3 && m->cols == 3) {
        return _determinant_3x3(m);
    }

    double result = 0;

    for (int i = 0; i < m->rows; i++) {
        double current_value = _get(m, i, 0);
        if (current_value != 0) {
            Matrix *complement = _new_complementary_matrix(m, i, 0);

            result += (i % 2 == 0 ? 1 : -1) * current_value * matrix_determinant(complement);
            delete_matrix(complement);
        } 
    }

    return result;
}

void delete_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->values[i]);
    }

    free(m->values);
    free(m);
}

