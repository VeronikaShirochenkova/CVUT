#include <stdio.h>
#include <stdlib.h>

#define MAX 10000
#define MIN -10000

/* The main program */
int main(int argc, char *argv[])
{
  int a = 0;
  int b = 0;

  int sum = 0;
  int diff= 0;
  int prod = 0;
  int quotient = 0;
  float average = 0;

  scanf("%d %d", &a, &b);

  if ( a >= MIN && a <= MAX && b >= MIN && b <= MAX) {

    printf("Desitkova soustava: %d %d\n", a, b);
    printf("Sestnactkova soustava: %x %x\n", a, b);

    sum = a + b;
    printf("Soucet: %d + %d = %d\n", a, b, sum);
    
    diff = a - b;
    printf("Rozdil: %d - %d = %d\n", a, b, diff);

    prod = a * b;
    printf("Soucin: %d * %d = %d\n", a, b, prod);

    if (b == 0) {
      printf("Nedefinovany vysledek!\n");
    }
    else {
      quotient = a / b;
      printf("Podil: %d / %d = %d\n", a, b, quotient);
    }

    average =(float)(a + b) / 2;
    printf("Prumer: %.1f\n", average);

  }
  else {
    printf("Vstup je mimo interval!\n");
  }
  
  return 0;
}
