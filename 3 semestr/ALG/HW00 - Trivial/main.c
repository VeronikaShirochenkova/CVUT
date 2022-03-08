#include <stdio.h>
#include <stdlib.h>
// max count of input numbers
#define MAX_SIZE 1000

// function that compares two numbers
int compare (const void *a, const void *b) {
  return *(int*)a -*(int*)b;
}

int main(int argc, char *argv[]) {

  int read = 0, counter = 0, number = 0, idx = 0;
  int array[MAX_SIZE] = {0};

  // reading input
  while ( (read = scanf("%d", &number)) > 0 ) {
    array[counter] = number;
    counter ++;
  }
  // sorting numbers
  qsort(array, counter, sizeof(int), compare);
  // printing numbers that occur more than once
  while (idx < counter) {
    if (array[idx] == array[idx + 1]) {
      int count = 0;
      while (array[idx] == array[idx + 1] && idx < counter) {
        count++;
        idx++;
      }
      if (count > 0) {
        printf("%d ", array[idx]);
      }
    }
    else {
      idx++;
    }
  }

  printf("\n");
  return 0;
}
