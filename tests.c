#include <stdio.h>
#include "matrix.h"
#include <assert.h>


void test_new_matrix() {
    Matrix *m = new_matrix(3, 3);

    matrix_set(m, 0, 0, 1.0);
    assert(1.0 == matrix_get(m, 0, 0));

    delete_matrix(m);
}

void test_new_matrix_with_values() {
    double values[3][3] = { 
        {1, 2, 3}, 
        {3, 2, 1}, 
        {4, 5, 6}
    };

    Matrix *m = new_matrix_with_values(3, 3, values);

    assert(1.0 == matrix_get(m, 0, 0));
    assert(2.0 == matrix_get(m, 0, 1));
    assert(3.0 == matrix_get(m, 0, 2));
    assert(3.0 == matrix_get(m, 1, 0));
    assert(2.0 == matrix_get(m, 1, 1));
    assert(1.0 == matrix_get(m, 1, 2));
    assert(4.0 == matrix_get(m, 2, 0));
    assert(5.0 == matrix_get(m, 2, 1));
    assert(6.0 == matrix_get(m, 2, 2));

    delete_matrix(m);
}

void test_new_identity_matrix() {
    Matrix *m = new_identity_matrix(5);

    assert(5 == m->rows);
    assert(5 == m->cols);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            assert((i == j ? 1 : 0) == matrix_get(m, i, j));
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

    Matrix *m = new_matrix_with_values(3, 2, values);
    Matrix *expected = new_matrix_with_values(2, 3, matrix_transposed_values);

    assert(1 == matrix_equals(expected, matrix_transpose(m)));

    delete_matrix(m);
    delete_matrix(expected);
}

void test_matrix_sum() {
    double a_values[3][2] = { 
        {1, 2}, 
        {3, 2}, 
        {4, 5}
    };
    Matrix *a = new_matrix_with_values(3, 2, a_values);

    double b_values[3][2] = { 
        {9, 8}, 
        {7, 8}, 
        {6, 5}
    };
    Matrix *b = new_matrix_with_values(3, 2, b_values);

    double expected_values[3][2] = { 
        {10, 10}, 
        {10, 10}, 
        {10, 10}
    };
    Matrix *expected = new_matrix_with_values(3, 2, expected_values);

    Matrix *result = matrix_sum(a, b);

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
    Matrix *a = new_matrix_with_values(3, 2, a_values);

    double b_values[3][2] = { 
        {9, 8}, 
        {7, 8}, 
        {6, 5}
    };
    Matrix *b = new_matrix_with_values(3, 2, b_values);

    double expected_values[3][2] = { 
        {-8, -6}, 
        {-4, -6}, 
        {-2, 0}
    };
    Matrix *expected = new_matrix_with_values(3, 2, expected_values);

    Matrix *result = matrix_subtract(a, b);

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
    Matrix *a = new_matrix_with_values(2, 3, a_values);

    double b_values[3][1] = { 
        {1}, 
        {2}, 
        {3}
    };
    Matrix *b = new_matrix_with_values(3, 1, b_values);

    double expected_values[2][1] = {
        {14},
        {32}
    };
    Matrix *expected = new_matrix_with_values(2, 1, expected_values);
    Matrix *result = matrix_multiply(a, b);

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
    Matrix *a = new_matrix_with_values(2, 2, a_values);

    double b_values[2][3] = { 
        {6, 4, -2}, 
        {0, 7, 1}, 
    };
    Matrix *b = new_matrix_with_values(2, 3, b_values);

    double expected_values[2][3] = {
        {18, 26, -4},
        {30, 13, -11}
    };
    Matrix *expected = new_matrix_with_values(2, 3, expected_values);
    Matrix *result = matrix_multiply(a, b);

    assert(1 == matrix_equals(expected, result));

    delete_matrix(a);
    delete_matrix(b);
    delete_matrix(result);
}

void test_determinant_2x2() {
    double values[2][2] = { 
        {3, 2}, 
        {5, -1}
    };
    Matrix *m = new_matrix_with_values(2, 2, values);

    assert(-13 == matrix_determinant(m));

    delete_matrix(m);
}

void test_determinant_3x3() {
    double values[3][3] = { 
        {4, -3, 5}, 
        {2, 1, 3}, 
        {4, 1, 0}
    };
    Matrix *m = new_matrix_with_values(3, 3, values);

    assert(-58 == matrix_determinant(m));

    delete_matrix(m);
}

void test_determinant_4x4() {
    double values[4][4] = { 
        {2, 3, -1, 2}, 
        {0, 4, -3, 5}, 
        {1, 2, 1, 3}, 
        {0, 4, 1, 0}
    };

    Matrix *m = new_matrix_with_values(4, 4, values);

    assert(-119 == matrix_determinant(m));
    delete_matrix(m);
}

void test_determinant_5x5() {
    double values[5][5] = { 
        {1, 0, 0, 3, 1}, 
        {0, 0, 0, 2, 0}, 
        {-1, 0, 2, 1, -2}, 
        {3, 1, 0, -2, 2}, 
        {2, 0, 1, 1, 0}
    };

    Matrix *m = new_matrix_with_values(5, 5, values);

    assert(6 == matrix_determinant(m));
    delete_matrix(m);
}

void test_determinant_6x6() {
    double values[6][6] = { 
        {-2, -1, 1, 3, 5, 7}, 
        {0, 2, 1, 3, 2, 4}, 
        {0, 0, 3, -1, 4, 8}, 
        {0, 0, 0, 1, 2, 3}, 
        {0, 0, 0, 1, 5, 4}, 
        {0, 0, 0, 0, 4, 2}, 
    };

    Matrix *m = new_matrix_with_values(6, 6, values);

    assert(-24 == matrix_determinant(m));
    delete_matrix(m);
}

int main() {
    test_new_matrix();
    test_new_matrix_with_values();
    test_new_identity_matrix();
    test_transpose_matrix();
    test_matrix_sum();
    test_matrix_subtract();
    test_matrix_multiply_1();
    test_matrix_multiply_2();
    test_determinant_2x2();
    test_determinant_3x3();
    test_determinant_4x4();
    test_determinant_5x5();
    test_determinant_6x6();

    printf("All tests passed successfully.");
}
