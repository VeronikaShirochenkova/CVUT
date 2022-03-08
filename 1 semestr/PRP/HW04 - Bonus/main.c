#include <stdio.h>
#include <stdlib.h>

#define SIZE 100
#define MIN 0
#define MAX 1000000

int main(int argc, char *argv[]) {

  // array for Sieve of Eratosthenes
  int sieve[MAX];
  // fill array with numbers 0 - 1 000 000
  for (int i = 0; i < MAX; i++) {
    sieve[i] = i;
  }
  // replace all non-prime numbers with zero
  int num = 2;
  while (num * num < MAX) {
    if (sieve[num] != 0) {
      int index = num * num;
      while (index < MAX) {
        sieve[index] = 0;
        index += num;
      }
    }
    num++;
  }
  next: while (1) {
    char read;
    char input[SIZE];
    int k = 0, counter = 0;
  
    // read number
    while ((read = getchar()) != EOF && k < SIZE-1 && read != '\n') {
      if (read < 48 || read > 57) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
      }
      input[k] = read;
      k++;
      counter++;
    }
    if (read == EOF ) break;
    // convert to int array
    int number[counter];
    for (int i = 0; i < counter; i++) {
      number[i]=input[i] - '0';
    }
    // if input number is 0
    if (counter == 1 && number[0] == 0) {
      goto next;
    } 
    // if input number is 1
    if (counter == 1 && number[0] == 1) {
      printf("Prvociselny rozklad cisla 1 je:\n1\n");
      goto next;
    }

    int number_cpy[SIZE] = {0};
    int idx = 2;
    int exponent = 0;

    printf("Prvociselny rozklad cisla ");
    for (int i = 0; i < counter; i++) {
      printf("%d", number[i]);
    }
    printf(" je:\n");
    int flag = 0;
    while (idx < MAX) {    
      if(sieve[idx] == 0) {
        idx++;
      }
      if (sieve[idx] != 0) {
        int modulo = 0;
        int n = 0;
        int div = 0;
        flag = 0; 
        for (int i = 0; i < counter; i++) {
          n = modulo * 10 + number[i];
          div = n / sieve[idx];
          number_cpy[i] = div;
          modulo = n % sieve[idx];
        }
      
        for (int i = 0; i < counter; i++) {
          flag += number_cpy[i];
        } 

        if(modulo == 0 ) {
          exponent ++;
          for (int j = 0; j < counter; j++) {
            number[j] = number_cpy[j];
          }       
        }
        if (modulo != 0) {      
          if(exponent > 0) {
            printf("%d", sieve[idx]);
            if(exponent > 1) {
              printf("^%d", exponent);
            }
            exponent = 0;
            if(flag != 0 ) {
              printf(" x ");
            }
          }
          idx++;
        }
      }
      if (flag == 0) break;
    }
    printf("\n");
  }
  return 0;
}
