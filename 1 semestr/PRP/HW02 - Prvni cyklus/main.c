#include <stdio.h>
#include <stdlib.h>
 
#define MIN -10000
#define MAX 10000
 
int main(int argc, char* argv[]) {

  int read, number, counter = 0, sum = 0;
  int positive = 0, negative = 0, even = 0, odd = 0;
  float percent_pos = 0, percent_neg = 0, percent_even = 0, percent_odd = 0;
  float average = 0;
  int max_number = -10000, min_number = 10000; // constants for searching max and min numbers
 
  while ( (read = scanf("%d", &number)) > 0 ) { 
    // intervals
    if (number >= MIN && number <= MAX) {
      counter += 1;
      if (counter == 1) {
        printf("%d", number);
      }         
      if (counter != 1) {
        printf(", ");
        printf("%d", number);
      }
    }
    else {
      printf("\nError: Vstup je mimo interval!\n");
      return 100;
    }
 
    // positive or negative
    if (number > 0) {
      positive += 1;
    }
    if (number < 0) {
      negative += 1;
    }
        
    // percentage of positive numbers
    percent_pos = (float) (positive * 100) / counter;
    // percentage of negative numbers
    percent_neg = (float) (negative * 100) / counter;
        
    // even or odd
    if (number % 2 == 0) {
      even += 1;
    }
    if (number % 2 != 0) {
      odd += 1;
    }
    // percentage of even numbers
    percent_even = (float) (even * 100) / counter;
    // percentage of odd numbers
    percent_odd = (float) (odd * 100) / counter;
 
    // average of all numbers
    sum += number;
    average = (float) sum / counter;
        
    // min and max number
    if (number <= min_number) {
      min_number = number;
    }
    if (number >= max_number) {
      max_number = number;
    }  
  }
 
  if (read == 0) {
    printf("Error: Vstup neni cislo!\n");
    return 100;
  } 
 
  printf("\nPocet cisel: %d\n", counter);
  printf("Pocet kladnych: %d\n", positive);
  printf("Pocet zapornych: %d\n", negative);
  printf("Procento kladnych: %.2f\n", percent_pos);
  printf("Procento zapornych: %.2f\n", percent_neg);
  printf("Pocet sudych: %d\n", even);
  printf("Pocet lichych: %d\n", odd);
  printf("Procento sudych: %.2f\n", percent_even);
  printf("Procento lichych: %.2f\n", percent_odd);
  printf("Prumer: %.2f\n", average);
  printf("Maximum: %d\n", max_number);
  printf("Minimum: %d\n", min_number);
 
  return 0;
}
