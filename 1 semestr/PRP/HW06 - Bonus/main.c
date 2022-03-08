#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define EXIT_SUCCESS 0
#define EXIT_FAIL 100

#define NO_SIGN ' '
#define MULT '*'
#define SUM '+'
#define DIFF '-'

#define INIT_SIZE 50

#define NO_IDX -1

enum mtrxs {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
    S, T, U, V, W, X, Y, Z, MATRIX_COUNT
};

const char mtrx_str[MATRIX_COUNT] = {
    [A] = 'A', [B] = 'B', [C] = 'C', [D] = 'D', [E] = 'E', [F] = 'F', 
    [G] = 'G', [H] = 'H', [I] = 'I', [J] = 'J', [K] = 'K', [L] = 'L', 
    [M] = 'M', [N] = 'N', [O] = 'O', [P] = 'P', [Q] = 'Q', [R] = 'R', 
    [S] = 'S', [T] = 'T', [U] = 'U', [V] = 'V', [W] = 'W', [X] = 'X', 
    [Y] = 'Y', [Z] = 'Z'
};

// matrix structure
typedef struct matrix {
    int row;
    int col;
    int capacity;
    int *data;
} matrix_t;

// function that finds a name of a matrix in an array of names
int find_name_in_array(const char *, char);
// function that read input data(matrix numbers)
bool read_input(matrix_t *);

// function that initializes all matricies in an array
void matrices_init(matrix_t *);
// function that free all matrices's data 
void matrices_free(matrix_t *);

// function that allocates memory for a matrix and copies existing values into it
bool matrix_create(matrix_t *matrices, matrix_t *matrix);
// function that allocates memory for matrix
bool matrix_enlarge(matrix_t *, int);
// function that rewrites values from one matrix to another
bool matrix_rewrite(matrix_t *, matrix_t *);
// function that printing the matrix
void matrix_print(matrix_t *);

// function that calls sum/difference/multiplication
bool make_operation(matrix_t *, matrix_t *, char);
// function that adds two matrices
bool sum(matrix_t *, matrix_t *);
// function that calculates the difference of matrices
bool difference(matrix_t *, matrix_t *);
// function that calculates the product of matrices
bool multiplication(matrix_t *, matrix_t *);


int main(int argc, char *argv[]) {

    matrix_t matrices[MATRIX_COUNT];
    matrices_init(matrices);

    if (!read_input(matrices)) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return EXIT_FAIL;
    }

    /* START */
    matrix_t matrix1, matrix2, matrix3;
    char sign1 = NO_SIGN, sign2 = NO_SIGN;

    // 1ST MATRIX
    if (!matrix_create(matrices, &matrix1)) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return EXIT_FAIL;
    }

    while (scanf(" %c", &sign1) != EOF) {
        // 2ND MATRIX
        if (!matrix_create(matrices, &matrix2)) {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return EXIT_FAIL;
        }

        if (sign1 == MULT) {
            if (!(make_operation(&matrix1, &matrix2, sign1))) {
                free(matrix1.data);
                free(matrix2.data);
                fprintf(stderr, "Error: Chybny vstup!\n");
                return EXIT_FAIL;
            }
        }
    NEXT:
        if (sign1 == SUM || sign1 == DIFF) {
            // 2ND SIGN
            if (scanf(" %c", &sign2) != EOF) {
                // 3RD MATRIX
                if (!matrix_create(matrices, &matrix3)) {
                    fprintf(stderr, "Error: Chybny vstup!\n");
                    return EXIT_FAIL;
                }

                if (sign2 == MULT) {
                    if (!(make_operation(&matrix2, &matrix3, sign2))) {
                        free(matrix3.data);
                        fprintf(stderr, "Error: Chybny vstup!\n");
                        return EXIT_FAIL;
                    }
                    free(matrix3.data);
                    goto NEXT;
                }
            }
            if (sign1 == SUM || sign1 == DIFF) {
                if (!(make_operation(&matrix1, &matrix2, sign1))) {
                    fprintf(stderr, "Error: Chybny vstup!\n");
                    return EXIT_FAIL;
                }
            }
            if (sign2 == SUM || sign2 == DIFF) {
                sign1 = sign2;
                sign2 = NO_SIGN;

                matrix_rewrite(&matrix2, &matrix3);
                free(matrix3.data);

                goto NEXT;
            }
        }
        free(matrix2.data);
    }

    /* FINISH */
    matrix_print(&matrix1);

    free(matrix1.data);
    matrices_free(matrices);

    return 0;
}

int find_name_in_array(const char *array, char name) {

    for (int i = 0; i < MATRIX_COUNT; i++) {
        if (array[i] == name) {
            return i;
        }
    }
    return NO_IDX;
}

bool read_input(matrix_t *matrices) {

    bool result = true;

    char name, num;
    int size = 0, number = 0;
    int mtrx_idx = -1, sign = 1;

    while ((name = getc(stdin)) != EOF) {
        if ((mtrx_idx = find_name_in_array(mtrx_str, name)) != NO_IDX) {

            size = 0;
            number = 0;
            sign = 1;

            matrices[mtrx_idx].row = 1;
            matrices[mtrx_idx].col = 1;
            if (!matrix_enlarge(matrices, mtrx_idx)) {
                result = false;
                return result;
            }

            while ((num = getc(stdin)) != '\n') {
                if (size >= (matrices[mtrx_idx].capacity - 2)) {
                    if (!matrix_enlarge(matrices, mtrx_idx)) {
                        result = false;
                        return result;
                    }
                }
                // if char is sign of number
                if (num == '-') {
                    sign = -1;
                }
                // if char is number
                if (num >= '0' && num <= '9') {
                    number = (number == 0) ? (num - '0') : (number * 10 + (num - '0'));
                }
                // if it's the end of number
                if (num == ' ') {
                    if (matrices[mtrx_idx].row == 1) {
                        matrices[mtrx_idx].col++;
                    }

                    matrices[mtrx_idx].data[size] = number * sign;
                    size++;
                    number = 0;
                    sign = 1;
                }
                if (num == ';') {
                    matrices[mtrx_idx].row++;
                    matrices[mtrx_idx].data[size] = number * sign;
                    size++;
                    number = 0;
                    sign = 1;
                    num = getc(stdin);
                }
                if (num == ']') {
                    matrices[mtrx_idx].data[size] = number * sign;
                    size++;
                    number = 0;
                    sign = 1;
                }
            }
        }
        else {
            break;
        }
    }
    return result;
}

void matrices_init(matrix_t *m) {

    for (int i = 0; i < MATRIX_COUNT; i++) {
        m[i].row = -1;
        m[i].col = -1;
        m[i].capacity = 0;
        m[i].data = NULL;
    }
}

void matrices_free(matrix_t *m) {

    for (int i = 0; i < MATRIX_COUNT; i++) {
        if (m[i].capacity != 0) {
            free(m[i].data);
        }
    }
}

bool matrix_create(matrix_t *matrices, matrix_t *matrix) {

    bool result = true;

    char mtrx = getc(stdin);
    int idx = find_name_in_array(mtrx_str, mtrx);

    if (idx == NO_IDX) {
        result = false;
        return result;
    }

    matrix->row = matrices[idx].row;
    matrix->col = matrices[idx].col;
    matrix->data = malloc(sizeof(int) * matrix->row * matrix->col);
    if (matrix->data == NULL) {
        result = false;
        return result;
    }

    int data_count = matrix->row * matrix->col;
    for (int i = 0; i < data_count; i++) {
        matrix->data[i] = matrices[idx].data[i];
    }

    return result;
}

bool matrix_enlarge(matrix_t *m, int idx) {
    bool result = false;
    int *data = NULL;
    int size = m[idx].capacity;

    if (size == 0) {
        data = malloc(sizeof(int) * INIT_SIZE);
        if (data == NULL) {
            return result;
        }
        size = INIT_SIZE;
    }
    else {
        size *= 2;
        data = realloc(m[idx].data, sizeof(int) * size);
        if (data == NULL) {
            return result;
        }
    }

    m[idx].data = data;
    m[idx].capacity = size;
    result = true;

    return result;
}

bool matrix_rewrite(matrix_t *matrix1, matrix_t *matrix2) {

    int result = false;

    matrix1->row = matrix2->row;
    matrix1->col = matrix2->col;

    int size = matrix1->row * matrix1->col;
    matrix1->data = realloc(matrix1->data, sizeof(int) * size);
    if (matrix1->data == NULL) {
        return result;
    }

    for (int i = 0; i < size; i++) {
        matrix1->data[i] = matrix2->data[i];
    }

    result = true;
    return result;
}

void matrix_print(matrix_t *matrix) {

    printf("[");
    int size = matrix->row * matrix->col;
    for (int r = 0; r < matrix->row; r++) {
        for (int c = 0; c < matrix->col; c++) {
            if (c == 0 && r == 0) {
                printf("%d", matrix->data[c + matrix->col * r]);
            }
            else {
                printf(" %d", matrix->data[c + matrix->col * r]);
            }
            if (c == (matrix->col - 1) && (c + matrix->col * r) != (size - 1)) {
                printf(";");
            }
        }
    }
    printf("]\n");
}

bool make_operation(matrix_t *matrix1, matrix_t *matrix2, char sign) {

    bool result = false;

    // MULTIPLICATION
    if (sign == MULT) {
        result = multiplication(matrix1, matrix2);
    }
    // SUMMATION
    if (sign == SUM) {
        result = sum(matrix1, matrix2);
    }
    // DIFFERENCE
    if (sign == DIFF) {
        result = difference(matrix1, matrix2);
    }

    return result;
}

bool sum(matrix_t *matrix1, matrix_t *matrix2) {

    bool result = true;
    if (matrix1->row == matrix2->row && matrix1->col == matrix2->col) {
        int size = matrix1->row * matrix1->col;
        for (int i = 0; i < size; i++) {
            matrix1->data[i] += matrix2->data[i];
        }
    }
    else {
        result = false;
    }
    return result;
}

bool difference(matrix_t *matrix1, matrix_t *matrix2) {

    bool result = true;

    if (matrix1->row == matrix2->row && matrix1->col == matrix2->col) {
        int size = matrix1->row * matrix1->col;
        for (int i = 0; i < size; i++) {
            matrix1->data[i] -= matrix2->data[i];
        }
    }
    else {
        result = false;
    }
    return result;
}

bool multiplication(matrix_t *matrix1, matrix_t *matrix2) {

    bool result = false;

    if (matrix1->col == matrix2->row) {
        int size = matrix1->row * matrix2->col;

        matrix_t matrix3;
        matrix3.row = matrix1->row;
        matrix3.col = matrix2->col;
        matrix3.data = malloc(sizeof(int) * size);  
        if (matrix3.data == NULL) {
            return result;
        }

        for (int a = 0; a < size; a++) {
            matrix3.data[a] = 0;
        }

        for (int i = 0; i < matrix1->row; i++) {
            for (int j = 0; j < matrix2->col; j++) {
                for (int k = 0; k < matrix1->col; k++) {
                    matrix3.data[i * matrix2->col + j] += matrix1->data[i * matrix1->col + k] * matrix2->data[k * matrix2->col + j];
                }
            }
        }

        free(matrix1->data);
        matrix1->row = matrix3.row;
        matrix1->col = matrix3.col;   
        matrix1->data = matrix3.data;

        result = true;
    }
    else {
        result = false;
    }

    return result;
}
