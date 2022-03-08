#include <stdio.h>
#include <stdlib.h>

#define MIN 0
#define MAX 1000000

void decompose(int long, int data[]);

int main(int argc, char *argv[]) {

  // input number
  long int number;
  // input control
  int read = 0;
  // array for Sieve of Eratosthenes
  int data[MAX] = {0};
  // fill array with numbers 0 - 1 000 000
  for (int i = 0; i < MAX; i++) {
    data[i] = i;
  }
  // replace all non-prime numbers with zero
  int num = 2;
  while (num * num < MAX) {
    if (data[num] != 0) {
      int idx = num * num;
      while (idx < MAX) {
        data[idx] = 0;
        idx += num;
      }
    }
    num++;
  }

  // read input numbers & decompose
  while ( (read = scanf("%ld", &number)) > 0 ) {
    // if number is negative
    if ( number < MIN ) { 
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    // if number is zero
    if ( number == MIN ) {
      return 0;
    }
    // if number is okay
    else {
      decompose(number, data);
    }
  }
  // if there is no input
  if ( read == 0 ) {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  return 0;
}

void decompose(int long number, int data[]) {

  int idx = 2;
  printf("Prvociselny rozklad cisla %ld je:\n", number);
  // special case for 1
  if (number == 1) {
    printf("%ld", number);
  }
  else {
    while (idx <= number) {
      if (data[idx] != 0) {
        int exponent = 0;
        while ((number % data[idx]) == 0) {
          number = number / data[idx];
          exponent++;
        }
        if (exponent > 0) {
          printf("%d", data[idx]);
          if (exponent > 1) {
            printf("^%d", exponent);
          }
          if (number > 1) {
            printf(" x ");
          }
        }
        idx++;
      }
      else {
        idx++;
      }
    }
  }
  printf("\n");
}
