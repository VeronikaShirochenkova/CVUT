#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int square_clean(char *, int, int, int, int);
int square_non_clean(char *, int, int, int, int);

int main(int argc, char *argv[])
{
  // cteni vstupni plochy
  int row, col;
  scanf("%d %d ", &row, &col);

  int size = row * col;
  char *array = malloc(sizeof(char) * size);
  for (int i = 0; i < size; i++) {
    scanf("%c ", &array[i]);
  }

  int max_square = 0;
  int last_elmnt = col * (row - 1) - 1;
  // pohyb po vnitrnim poli
  for (int index = col + 1; index < last_elmnt; index++) {
    if ((index % col) > 0 && (index % col) < (col - 1)) {
      if (array[index - 1] != 'X' && array[index] != 'X' && array[index + 1] != 'X') {
        // stredni cast od "1" do "sirka-1"
        int edge =(col - (index % col) - 1);
        for (int middle_part = 1; middle_part <= edge; middle_part++) {
          int square = 0;
          // stred leve casti
          int left_part = index - 1;
          // sred prave casti
          int right_part = index + middle_part;
          // test na "X" a "o" v stredni casti
          int yellow_middle = 0;
          bool stop = false;
          for (int i = index; i < right_part; i++) {
            if (array[i] == 'X') {
              stop = true;
              break;
            }
            if (array[i] == 'o') {
              yellow_middle++;
            }
            if (yellow_middle > 1) {
              stop = true;
              break;
            }
          }
          // jestli v stredni je "X" nebo vetsi nez jedna "o"
          if (stop) { break; }
          square = (yellow_middle == 1) ? square_clean(array, size, col, left_part, middle_part) : square_non_clean(array, size, col, left_part, middle_part);
          max_square = MAX(square, max_square);
        }
      }
    }
  }
  printf("%d\n", max_square);
  free(array);
  return 0;
}

int square_clean(char *array, int size, int col, int left_part, int middle_part) {
  int sq = 0;
  int up = 0;
  int down = 0;
  // vertikalni casti horu
  for (int j = left_part; j >= 0; j -= col) {
    if (array[j] == 'X' || array[j + middle_part + 1] == 'X' || array[j] == 'o' || array[j + middle_part + 1] == 'o') {
      break;
    }
    else {
      sq += 2;
      up++;
    }
  }
  // vertikalni casti dolu
  if (up > 1) {
    for (int k = left_part; k < size; k += col) {
      if (array[k] == 'X' || array[k + middle_part + 1] == 'X' || array[k] == 'o' || array[k + middle_part + 1] == 'o') {
        break;
      }
      else {
        sq += 2;
        down++;
      }
    }
  }

  sq = (down < 2 || up < 2) ? 0 : (sq + middle_part - 2);
  return sq;
}

int square_non_clean(char *array, int size, int col, int left_part, int middle_part) {
  int sq = 0;

  int up = 0;
  int down = 0;

  int count_yellow_up = 0;
  int count_yellow_down = 0;

  int up_yellow_square = 0;
  int up_yellow = 0;

  int down_yellow_square = 0;
  int down_yellow = 0;

  // vertikalni casti horu
  for (int a = left_part; a >= 0; a -= col) {
    if (array[a] == 'X' || array[a + middle_part + 1] == 'X') {
      break;
    }
    if (array[a] == 'o' && array[a + middle_part + 1] == 'o') {
      break;
    }
    if (array[a] == 'o' || array[a + middle_part + 1] == 'o') {
      if (count_yellow_up == 1) {
        break;
      }
      else {
        count_yellow_up++;
        up_yellow_square += 2;
      }
    }
    if (array[a] == '.' && array[a + middle_part + 1] == '.') {
      if (count_yellow_up == 0) {
        up++;
        up_yellow_square += 2;
        up_yellow++;
      }
      else {
        up_yellow_square += 2;
        up_yellow++;
      }
    }
  }
  // vertikalni casti dolu
  for (int b = left_part + col; b < size; b += col) {
    if (array[b] == 'X' || array[b + middle_part + 1] == 'X') {
      break;
    }
    if (array[b] == 'o' && array[b + middle_part + 1] == 'o') {
      break;
    }
    if (array[b] == 'o' || array[b + middle_part + 1] == 'o') {
      if (count_yellow_down == 1) {
        break;
      }
      else {
        count_yellow_down++;
        down_yellow_square += 2;
        down_yellow++;
      }
    }
    if (array[b] == '.' && array[b + middle_part + 1] == '.') {
      if (count_yellow_down == 0) {
        down++;
        down_yellow_square += 2;
        down_yellow++;
      }
      else {
        down_yellow_square += 2;
        down_yellow++;
      }
    }
  }
  // clean up part + clean down part
  int square1 = (up > 1 && down > 0) ? (up*2 + down*2) : 0;
  // clean up part + yellow down part
  int square2 = (up > 1 && down_yellow > 0) ? (up*2 + down_yellow_square) : 0;
  // yellow up part + clean down part
  int square3 = (up_yellow > 1 && down > 0) ? (up_yellow_square + down*2) : 0;
  //printf("sq1: %d, sq2: %d, sq3: %d\n", square1, square2, square3);

  sq = (square1 == 0 && square2 == 0 && square3 == 0) ? 0 : (MAX(MAX(square1, square2), square3) + middle_part);
  return sq;
}
