#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

MatrixNumericResult _build_numeric_result(MatrixResultCode code, double value) {
    MatrixNumericResult result;
    result.success = code == MATRIX_SUCCESS_CODE ? 1 : 0;
    result.code = code;
    result.value = value;
    return result;
}

MatrixNumericResult _succeeded_numeric_result(double value) {
    return _build_numeric_result(MATRIX_SUCCESS_CODE, value);
}

MatrixNumericResult _failed_numeric_result(MatrixResultCode error_code) {
    return _build_numeric_result(error_code, -1);
}

MatrixResult _build_matrix_result(MatrixResultCode code, Matrix *value) {
    MatrixResult result;
    result.success = code == MATRIX_SUCCESS_CODE ? 1 : 0;
    result.code = code;
    result.value = value;
    return result;
}

MatrixResult _succeeded_matrix_result(Matrix *value) {
    return _build_matrix_result(MATRIX_SUCCESS_CODE, value);
}

MatrixResult _failed_matrix_result(MatrixResultCode error_code) {
    return _build_matrix_result(error_code, NULL);
}

MatrixResult new_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        return _failed_matrix_result(MATRIX_DIMENSIONS_MUST_BE_POSITIVE);
    }

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

    return _succeeded_matrix_result(m);
}

double _get(Matrix *m, int row, int col) {
    return m->values[row][col];
}

void _set(Matrix *m, int row, int col, double value) {
    m->values[row][col] = value;
}

MatrixResult new_matrix_with_values(int rows, int cols, double values[rows][cols]) {
    MatrixResult result = new_matrix(rows, cols);

    if (result.success) {
        Matrix *m = result.value;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                _set(m, i, j, values[i][j]);
            }
        }
    }

    return result;
}

MatrixResult new_identity_matrix(int n) {
    MatrixResult result = new_matrix(n, n);

    if (result.success) {
        Matrix *m = result.value;

        for (int i = 0; i < n; i++) {
            _set(m, i, i, 1);
        }
    }

    return result;
}

MatrixResult matrix_transpose(Matrix *m) {
    if (m == NULL) return _failed_matrix_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);

    Matrix *result = new_matrix(m->cols, m->rows).value;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            _set(result, j, i, _get(m, i, j));
        }
    }

    return _succeeded_matrix_result(result);
}

MatrixNumericResult matrix_get(Matrix *m, int row, int col) {
    if (row > 0 && row <= m->rows && col > 0 && col <= m->cols) {
        return _succeeded_numeric_result(_get(m, row - 1, col - 1));
    } 

    return _failed_numeric_result(MATRIX_INDEX_ARGUMENT_OUT_OF_BOUNDS);
}

MatrixNumericResult matrix_set(Matrix *m, int row, int col, double value) {
    if (row > 0 && row <= m->rows && col > 0 && col <= m->cols) {
        _set(m, row - 1, col - 1, value);
        return _succeeded_numeric_result(value);
    } 

    return _failed_numeric_result(MATRIX_INDEX_ARGUMENT_OUT_OF_BOUNDS);
}

int matrix_equals(Matrix *a, Matrix *b) {
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
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

MatrixResult matrix_sum(Matrix *a, Matrix *b) {
    if (a == NULL || b == NULL) {
        return _failed_matrix_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);
    }

    if (a->rows != b->rows || a->cols != b->cols) {
        return _failed_matrix_result(MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUM);
    }

    Matrix *result = new_matrix(a->rows, a->cols).value;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            _set(result, i, j, _get(a, i, j) + _get(b, i, j));
        }
    }

    return _succeeded_matrix_result(result);
}

MatrixResult matrix_subtract(Matrix *a, Matrix *b) {
    if (a == NULL || b == NULL) {
        return _failed_matrix_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);
    }

    if (a->rows != b->rows || a->cols != b->cols) {
        return _failed_matrix_result(MATRIX_DIMENSIONS_MUST_BE_EQUALS_TO_SUBTRACT);
    }

    Matrix *result = new_matrix(a->rows, a->cols).value;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            _set(result, i, j, _get(a, i, j) - _get(b, i, j));
        }
    }

    return _succeeded_matrix_result(result);
}

float _multiply_row_by_col(Matrix *a, int a_row, Matrix *b, int b_col) {
    int size = a->cols;
    double result = 0;

    for (int i = 0; i < size; i++) {
        result += _get(a, a_row, i) * _get(b, i, b_col);
    }

    return result;
}

MatrixResult matrix_multiply(Matrix *a, Matrix *b) {
    if (a == NULL || b == NULL) return _failed_matrix_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);
    if (a->cols != b->rows) return _failed_matrix_result(MATRIX_INVALID_DIMENSIONS_TO_MULTIPLY);

    Matrix *result = new_matrix(a->rows, b->cols).value;

    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            _set(result, i, j, _multiply_row_by_col(a, i, b, j));
        }
    }

    return _succeeded_matrix_result(result);
}

MatrixNumericResult _determinant_2x2(Matrix *m) {
    return _succeeded_numeric_result(
        _get(m, 0, 0) * _get(m, 1, 1) - _get(m, 0, 1) * _get(m, 1, 0)
    );
}

MatrixNumericResult _determinant_3x3(Matrix *m) {
    double diagonal_1 = _get(m, 0, 0) * _get(m, 1, 1) * _get(m, 2, 2);
    double diagonal_2 = _get(m, 0, 1) * _get(m, 1, 2) * _get(m, 2, 0);
    double diagonal_3 = _get(m, 0, 2) * _get(m, 1, 0) * _get(m, 2, 1);
    double diagonal_4 = _get(m, 0, 2) * _get(m, 1, 1) * _get(m, 2, 0);
    double diagonal_5 = _get(m, 0, 0) * _get(m, 1, 2) * _get(m, 2, 1);
    double diagonal_6 = _get(m, 2, 2) * _get(m, 1, 0) * _get(m, 0, 1);

    return _succeeded_numeric_result(
        diagonal_1 + diagonal_2 + diagonal_3 - (
            diagonal_4 + diagonal_5 + diagonal_6
        )
    );
}

Matrix* _new_complementary_matrix(Matrix *m, int row_to_remove, int col_to_remove) {
    Matrix *result = new_matrix(m->rows - 1, m->cols - 1).value;

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


MatrixNumericResult matrix_determinant_laplace(Matrix *m) {
    if (m == NULL) return _failed_numeric_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);

    if (m->rows != m->cols) {
        return _failed_numeric_result(MATRIX_SHOULD_BE_SQUARE_TO_CALC_DETERMINANT);
    }

    if (m->rows == 1) {
        return _succeeded_numeric_result(_get(m, 0, 0));
    }

    if (m->rows == 2) {
        return _determinant_2x2(m);
    }

    if (m->rows == 3) {
        return _determinant_3x3(m);
    }

    double result = 0;

    for (int i = 0; i < m->rows; i++) {
        double current_value = _get(m, i, 0);

        if (current_value != 0) {
            Matrix *complement = _new_complementary_matrix(m, i, 0);
            MatrixNumericResult complement_det = matrix_determinant_laplace(complement);

            if (!complement_det.success) {
                return _failed_numeric_result(MATRIX_INTERNAL_ERROR);
            }

            result += (i % 2 == 0 ? 1 : -1) * current_value * complement_det.value;
            delete_matrix(complement);
        } 
    }

    return _succeeded_numeric_result(result);
}

Matrix* matrix_copy(Matrix *m) {
    if (m == NULL) return NULL;

    Matrix *copy = new_matrix(m->rows, m->cols).value;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m-> cols; j++) {
            _set(copy, i, j, _get(m, i, j));
        }
    }

    return copy;
}

MatrixNumericResult matrix_determinant_lu_decomposition(Matrix *m) {
    if (m == NULL) return _failed_numeric_result(MATRIX_ARGUMENTS_MUST_NOT_BE_NULL);

    if (m->rows != m->cols) {
        return _failed_numeric_result(MATRIX_SHOULD_BE_SQUARE_TO_CALC_DETERMINANT);
    }

    if (m->rows == 1) {
        return _succeeded_numeric_result(_get(m, 0, 0));
    }

    Matrix *copy = matrix_copy(m);

    for (int k = 0; k < m->rows - 1; k++) {
        double ref = _get(copy, k, k);

        if (ref == 0) {
            int col = k + 1;

            for (col; col < m->cols; col++) {
                if (_get(copy, k, col) != 0) {
                    ref = _get(copy, k, col);
                    break;
                }
            }

            if (ref == 0) return _succeeded_numeric_result(0);

            for (int row = 0; row < copy->rows; row++) {
                _set(copy, row, k, _get(copy, row, k) + _get(copy, row, col));
            }
        }

        for (int i = k + 1; i < m->rows; i++) {
            double value_to_set_zero = _get(copy, i, k);

            if (value_to_set_zero != 0) {
                double f = value_to_set_zero / ref * -1;
                for (int j = 0; j < m->cols; j++) {
                    _set(copy, i, j, _get(copy, i, j) + _get(copy, k, j) * f);
                }
            }
        }
    }

    double det = 1;

    for (int i = 0; i < copy->rows; i++) {
        det *= _get(copy, i, i );
    }

    return _succeeded_numeric_result(det);
}

void delete_matrix(Matrix *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->values[i]);
    }

    free(m->values);
    free(m);
}
