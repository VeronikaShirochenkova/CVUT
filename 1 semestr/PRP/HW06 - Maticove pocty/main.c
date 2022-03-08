#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NO_SIGN ' '
#define MULT '*'
#define SUM '+'
#define DIFF '-'

// function that allocate memory for matrix 
void matrix_alloc(int rows, int columns, int **matrix);
// function that reads the matrix
int matrix_read(int rows, int columns, int *matrix);
// function that prints the matrix
void matrix_print(int rows, int columns, int **matrix);
// function that rewrite 2nd matrix into the 1st
void matrix_rewrite(int **matrix1, int rows2, int columns2, int *matrix2);

// function that calls sum/difference/multiplication
bool make_operation(
    int rows1, int columns1, int **matrix1, 
    int rows2, int columns2, int *matrix2, 
    char sign);

// function that adds two matrices
bool sum(
    int rows1, int columns1, int **matrix1,
    int rows2, int columns2, int *matrix2
    );

// function that calculates the difference of matrices
bool difference(
    int rows1, int columns1, int **matrix1,
    int rows2, int columns2, int *matrix2
    );

// function that calculates the product of matrices
bool multiplication(
    int rows1, int columns1, int **matrix1,
    int rows2, int columns2, int *matrix2
    );


int main(int argc, char *argv[]) {
  
  int row1 = 0, col1 = 0;
  int row2 = 0, col2 = 0;
  int row3 = 0, col3 = 0;
  int *matrix1, *matrix2, *matrix3;

  char sign1 = NO_SIGN;
  char sign2 = NO_SIGN;
  
  // 1ST MATRIX
  if (!(scanf("%d %d ", &row1, &col1))) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  matrix_alloc(row1, col1, &matrix1);

  if (!(matrix_read(row1, col1, matrix1))) {
    free(matrix1);
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }

  while (scanf(" %c", &sign1) != EOF) {
    // 2ND MATRIX
    if (!(scanf("%d %d ", &row2, &col2))) {
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    matrix_alloc(row2, col2, &matrix2);
    if (!(matrix_read(row2, col2, matrix2))) {
      free(matrix2);
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }

    if (sign1 == MULT) {
      if (!(make_operation(row1, col1, &matrix1, row2, col2, matrix2, sign1))) {
        free(matrix1);
        free(matrix2);
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
      }
      col1 = col2;
    }
    NEXT: 
    if (sign1 == SUM || sign1 == DIFF) {
      // 2ND SIGN
      if (scanf(" %c", &sign2) != EOF) {
        // 3RD MATRIX
        if (!(scanf("%d %d ", &row3, &col3))) {
          fprintf(stderr, "Error: Chybny vstup!\n");
          return 100;
        }
        matrix_alloc(row3, col3, &matrix3);
        if (!(matrix_read(row3, col3, matrix3))) {
          free(matrix3);
          fprintf(stderr, "Error: Chybny vstup!\n");
          return 100;
        }

        if (sign2 == MULT) {
          if (!(make_operation(row2, col2, &matrix2, row3, col3, matrix3, sign2))) {
            free(matrix3);
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
          }

          col2 = col3;
          free(matrix3);

          goto NEXT;
        }      
      }
      if (sign1 == SUM || sign1 == DIFF) {
        if (!(make_operation(row1, col1, &matrix1, row2, col2, matrix2, sign1))) {
          fprintf(stderr, "Error: Chybny vstup!\n");
          return 100;
        }
      }
      if (sign2 == SUM || sign2 == DIFF) {
        sign1 = sign2;
        sign2 = NO_SIGN;

        matrix_rewrite(&matrix2, row3, col3, matrix3);
        row2 = row3;
        col2 = col3;
 
        free(matrix3);
        goto NEXT;
      }
    }
    free(matrix2);
  }
  matrix_print(row1, col1, &matrix1);
  free(matrix1);
  return 0;
}

void matrix_alloc(int rows, int columns, int **matrix) {
   
  (*matrix) = malloc(sizeof(int) * rows * columns);
}

int matrix_read(int rows, int columns, int *matrix) {

  bool result = true;

  for (int i = 0; i < rows * columns; i++) {
    if (scanf("%d ", &matrix[i]) != 1) {
      result = false;
      break;
    }
  }
  return result;
}

void matrix_print(int rows, int columns, int **matrix) {

  printf("%d %d\n", rows, columns);

  for (int i = 0; i < (rows * columns); i++) {

    if((i % columns) == (columns - 1)) {
      printf("%d", (*matrix)[i]);
    }
    else {
      printf("%d ", (*matrix)[i]);
    }

    if ((i % columns) == (columns - 1)) {
      printf("\n");
    }
  }

}

void matrix_rewrite(int **matrix1, int rows2, int columns2, int *matrix2) {
   int size = rows2 * columns2;
  (*matrix1) = realloc((*matrix1), sizeof(int) * size);

  for(int i = 0; i < size; i++) {
    (*matrix1)[i] = matrix2[i];
  }

}

bool make_operation(int rows1, int columns1, int **matrix1, int rows2, int columns2, int *matrix2, char sign) {

  bool result = false;

  // MULTIPLICATION 
  if (sign == MULT) {
    result = multiplication(rows1, columns1, matrix1, rows2, columns2, matrix2);
  }
  // SUMMATION
  if (sign == SUM) {
    result = sum(rows1, columns1, matrix1, rows2, columns2, matrix2);
  }
  // DIFFERENCE
  if (sign == DIFF) {
    result = difference(rows1, columns1, matrix1, rows2, columns2, matrix2);
  }
  
  return result;
}

bool sum(int rows1, int columns1, int **matrix1, int rows2, int columns2, int *matrix2) {

  bool result = true;

  if (rows1 == rows2 && columns1 == columns2) {
    int size = rows1 * columns1;
    for (int i = 0; i < size; i++) {
        (*matrix1)[i] += matrix2[i];
    }
  }
  else {
    result = false;
  }
  return result;
}

bool difference(int rows1, int columns1, int **matrix1, int rows2, int columns2, int *matrix2) {
  
  bool result = true;

  if (rows1 == rows2 && columns1 == columns2) {
    int size = rows1 * columns1;
    
    for (int i = 0; i < size; i++) {
      (*matrix1)[i] -= matrix2[i];
    }

  }
  else {
    result = false;
  }

  return result;
}

bool multiplication(int rows1, int columns1, int **matrix1, int rows2, int columns2, int *matrix2) {
  
  bool result = true;
  
  if (columns1 == rows2) {
    int size = rows1 * columns2;
    int *matrix3 = malloc(sizeof(int) * size);

    for (int a = 0; a < size; a++) {
      matrix3[a] = 0;
    }

    for (int i = 0; i < rows1; i++) {
      for (int j = 0; j < columns2; j++) {
        for (int k = 0; k < columns1; k++) {
          matrix3[i * columns2 + j] += (*matrix1)[ i * columns1 + k ] * matrix2[ k * columns2 + j];
        } 
      }
    }

    free((*matrix1));
    (*matrix1) = matrix3;

  }
  else {
    result = false;
  }
  
  return result;
}
