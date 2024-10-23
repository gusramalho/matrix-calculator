#include <stdio.h>
#include "matrix.h"
#include <assert.h>

void test_new_matrix() {
    Matrix *m = new_matrix(3, 3).value;

    matrix_set(m, 1, 1, 1.0);
    assert(1.0 == matrix_get(m, 1, 1).value);

    delete_matrix(m);
}

void test_matrix_get_and_set() {
    Matrix *m = new_matrix(2, 2).value;

    matrix_set(m, 1, 1, 30);
    matrix_set(m, 1, 2, 20);
    matrix_set(m, 2, 1, 1);
    matrix_set(m, 2, 2, 2);
    
    assert(30 == matrix_get(m, 1, 1).value);
    assert(20 == matrix_get(m, 1, 2).value);
    assert(1 == matrix_get(m, 2, 1).value);
    assert(2 == matrix_get(m, 2, 2).value);

    delete_matrix(m);
}

void test_new_matrix_with_values() {
    double values[3][3] = { 
        {1, 2, 3}, 
        {3, 2, 1}, 
        {4, 5, 6}
    };

    Matrix *m = new_matrix_with_values(3, 3, values).value;

    assert(1.0 == matrix_get(m, 1, 1).value);
    assert(2.0 == matrix_get(m, 1, 2).value);
    assert(3.0 == matrix_get(m, 1, 3).value);
    assert(3.0 == matrix_get(m, 2, 1).value);
    assert(2.0 == matrix_get(m, 2, 2).value);
    assert(1.0 == matrix_get(m, 2, 3).value);
    assert(4.0 == matrix_get(m, 3, 1).value);
    assert(5.0 == matrix_get(m, 3, 2).value);
    assert(6.0 == matrix_get(m, 3, 3).value);

    delete_matrix(m);
}

void test_new_identity_matrix() {
    Matrix *m = new_identity_matrix(5).value;

    assert(5 == m->rows);
    assert(5 == m->cols);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            assert((i == j ? 1 : 0) == matrix_get(m, i + 1, j + 1).value);
        }
    }

    delete_matrix(m);
}

void test_transpose_matrix() {
    double values[3][2] = { 
        {1, 2}, 
        {3, 2}, 
        {4, 5}
    };

    double matrix_transposed_values[2][3] = { 
        {1, 3, 4}, 
        {2, 2, 5}
    };

    Matrix *m = new_matrix_with_values(3, 2, values).value;
    Matrix *expected = new_matrix_with_values(2, 3, matrix_transposed_values).value;

    assert(1 == matrix_equals(expected, matrix_transpose(m).value));

    delete_matrix(m);
    delete_matrix(expected);
}

void test_matrix_sum() {
    double a_values[3][2] = { 
        {1, 2}, 
        {3, 2}, 
        {4, 5}
    };
    Matrix *a = new_matrix_with_values(3, 2, a_values).value;

    double b_values[3][2] = { 
        {9, 8}, 
        {7, 8}, 
        {6, 5}
    };
    Matrix *b = new_matrix_with_values(3, 2, b_values).value;

    double expected_values[3][2] = { 
        {10, 10}, 
        {10, 10}, 
        {10, 10}
    };
    Matrix *expected = new_matrix_with_values(3, 2, expected_values).value;

    Matrix *result = matrix_sum(a, b).value;

    assert(1 == matrix_equals(expected, result));

    delete_matrix(a);
    delete_matrix(b);
    delete_matrix(expected);
}

void test_matrix_subtract() {
    double a_values[3][2] = { 
        {1, 2}, 
        {3, 2}, 
        {4, 5}
    };
    Matrix *a = new_matrix_with_values(3, 2, a_values).value;

    double b_values[3][2] = { 
        {9, 8}, 
        {7, 8}, 
        {6, 5}
    };
    Matrix *b = new_matrix_with_values(3, 2, b_values).value;

    double expected_values[3][2] = { 
        {-8, -6}, 
        {-4, -6}, 
        {-2, 0}
    };
    Matrix *expected = new_matrix_with_values(3, 2, expected_values).value;

    Matrix *result = matrix_subtract(a, b).value;

    assert(1 == matrix_equals(expected, result));

    delete_matrix(a);
    delete_matrix(b);
    delete_matrix(expected);
}

void test_matrix_multiply_1() {
    double a_values[2][3] = { 
        {1, 2, 3}, 
        {4, 5, 6}
    };
    Matrix *a = new_matrix_with_values(2, 3, a_values).value;

    double b_values[3][1] = { 
        {1}, 
        {2}, 
        {3}
    };
    Matrix *b = new_matrix_with_values(3, 1, b_values).value;

    double expected_values[2][1] = {
        {14},
        {32}
    };
    Matrix *expected = new_matrix_with_values(2, 1, expected_values).value;
    Matrix *result = matrix_multiply(a, b).value;

    assert(1 == matrix_equals(expected, result));

    delete_matrix(a);
    delete_matrix(b);
    delete_matrix(result);
}

void test_matrix_multiply_2() {
    double a_values[2][2] = { 
        {3, 2}, 
        {5, -1}
    };
    Matrix *a = new_matrix_with_values(2, 2, a_values).value;

    double b_values[2][3] = { 
        {6, 4, -2}, 
        {0, 7, 1}, 
    };
    Matrix *b = new_matrix_with_values(2, 3, b_values).value;

    double expected_values[2][3] = {
        {18, 26, -4},
        {30, 13, -11}
    };
    Matrix *expected = new_matrix_with_values(2, 3, expected_values).value;
    Matrix *result = matrix_multiply(a, b).value;

    assert(1 == matrix_equals(expected, result));

    delete_matrix(a);
    delete_matrix(b);
    delete_matrix(result);
}

void test_determinant_2x2_laplace() {
    double values[2][2] = { 
        {3, 2}, 
        {5, -1}
    };
    Matrix *m = new_matrix_with_values(2, 2, values).value;
    MatrixNumericResult result = matrix_determinant_laplace(m);

    assert(1 == result.success);
    assert(-13 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_2x2_lu_decomposition() {
    double values[2][2] = { 
        {3, 2}, 
        {5, -1}
    };
    Matrix *m = new_matrix_with_values(2, 2, values).value;
    MatrixNumericResult result = matrix_determinant_lu_decomposition(m);

    assert(1 == result.success);
    assert(-13 >= result.value && result.value < -12.99);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_3x3_laplace() {
    double values[3][3] = { 
        {4, -3, 5}, 
        {2, 1, 3}, 
        {4, 1, 0}
    };
    Matrix *m = new_matrix_with_values(3, 3, values).value;
    MatrixNumericResult result = matrix_determinant_laplace(m);

    assert(1 == result.success);
    assert(-58 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_3x3_lu_decomposition() {
    double values[3][3] = { 
        {4, -3, 5}, 
        {2, 1, 3}, 
        {4, 1, 0}
    };
    Matrix *m = new_matrix_with_values(3, 3, values).value;
    MatrixNumericResult result = matrix_determinant_lu_decomposition(m);

    assert(1 == result.success);
    assert(-58 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_4x4_laplace() {
    double values[4][4] = { 
        {2, 3, -1, 2}, 
        {0, 4, -3, 5}, 
        {1, 2, 1, 3}, 
        {0, 4, 1, 0}
    };

    Matrix *m = new_matrix_with_values(4, 4, values).value;
    MatrixNumericResult result = matrix_determinant_laplace(m);

    assert(1 == result.success);
    assert(-119 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_4x4_lu_decomposition() {
    double values[4][4] = { 
        {2, 3, -1, 2}, 
        {0, 4, -3, 5}, 
        {1, 2, 1, 3}, 
        {0, 4, 1, 0}
    };

    Matrix *m = new_matrix_with_values(4, 4, values).value;
    MatrixNumericResult result = matrix_determinant_lu_decomposition(m);

    assert(1 == result.success);
    assert(-119 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_5x5_laplace() {
    double values[5][5] = { 
        {1, 0, 0, 3, 1}, 
        {0, 0, 0, 2, 0}, 
        {-1, 0, 2, 1, -2}, 
        {3, 1, 0, -2, 2}, 
        {2, 0, 1, 1, 0}
    };

    Matrix *m = new_matrix_with_values(5, 5, values).value;
    MatrixNumericResult result = matrix_determinant_laplace(m);

    assert(1 == result.success);
    assert(6 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_5x5_lu_decomposition() {
    double values[5][5] = { 
        {1, 0, 0, 3, 1}, 
        {0, 0, 0, 2, 0}, 
        {-1, 0, 2, 1, -2}, 
        {3, 1, 0, -2, 2}, 
        {2, 0, 1, 1, 0}
    };

    Matrix *m = new_matrix_with_values(5, 5, values).value;
    MatrixNumericResult result = matrix_determinant_lu_decomposition(m);

    assert(1 == result.success);
    assert(6 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_6x6_laplace() {
    double values[6][6] = { 
        {-2, -1, 1, 3, 5, 7}, 
        {0, 2, 1, 3, 2, 4}, 
        {0, 0, 3, -1, 4, 8}, 
        {0, 0, 0, 1, 2, 3}, 
        {0, 0, 0, 1, 5, 4}, 
        {0, 0, 0, 0, 4, 2}, 
    };

    Matrix *m = new_matrix_with_values(6, 6, values).value;
    MatrixNumericResult result = matrix_determinant_laplace(m);

    assert(1 == result.success);
    assert(-24 == result.value);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

void test_determinant_6x6_lu_decomposition() {
    double values[6][6] = { 
        {-2, -1, 1, 3, 5, 7}, 
        {0, 2, 1, 3, 2, 4}, 
        {0, 0, 3, -1, 4, 8}, 
        {0, 0, 0, 1, 2, 3}, 
        {0, 0, 0, 1, 5, 4}, 
        {0, 0, 0, 0, 4, 2}, 
    };

    Matrix *m = new_matrix_with_values(6, 6, values).value;
    MatrixNumericResult result = matrix_determinant_lu_decomposition(m);

    assert(1 == result.success);
    assert(-24 >= result.value && result.value < -23.99);
    assert(MATRIX_SUCCESS_CODE == result.code);

    delete_matrix(m);
}

int main() {
    test_new_matrix();
    test_matrix_get_and_set();
    test_new_matrix_with_values();
    test_new_identity_matrix();
    test_transpose_matrix();
    test_matrix_sum();
    test_matrix_subtract();
    test_matrix_multiply_1();
    test_matrix_multiply_2();
    test_determinant_2x2_laplace();
    test_determinant_2x2_lu_decomposition();
    test_determinant_3x3_laplace();
    test_determinant_3x3_lu_decomposition();
    test_determinant_4x4_laplace();
    test_determinant_4x4_lu_decomposition();
    test_determinant_5x5_laplace();
    test_determinant_5x5_lu_decomposition();
    test_determinant_6x6_laplace();
    test_determinant_6x6_lu_decomposition();

    printf("All tests passed successfully.");
}
