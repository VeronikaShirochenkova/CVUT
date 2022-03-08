#include <stdio.h>
#include <stdlib.h>

// intervals
#define MINIMUM 3
#define MAXIMUM 69
#define MIN_FENCE 0
// parts of house
int roof(int);
int house(int, int);
int house_with_fence(int, int, int);
int wood_fence(int, int, int);

int main(int argc, char *argv[]) {

  int width, height, fence, read;

  // input 2 values
  if ((read = scanf("%d %d", &width, &height)) == 2) {
    // optional task
    if ((read = scanf("%d", &fence)) == 1) {
      if (width < MINIMUM || width > MAXIMUM || height < MINIMUM || height > MAXIMUM) {
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return 101;
      }
      // if intervals are okay
      else {
        if (width % 2 != 0) {
          if (width == height) {
            if (fence < height && fence > MIN_FENCE) {
              roof(width);
              house_with_fence(width, height, fence);
            }
            else {
              fprintf(stderr, "Error: Neplatna velikost plotu!\n");
              return 103;
            }
          }
        }
        else {
          fprintf(stderr, "Error: Sirka neni liche cislo!\n");
          return 102;
        }
      }
    }
    // mandatory task
    else {
      if ((read = scanf("%d", &fence)) == 0) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
      }
      else {
        // if intervals are okay
        if (width >= MINIMUM && width <= MAXIMUM && height >= MINIMUM && height <= MAXIMUM) {
          if (width % 2 != 0) {
            roof(width);
            house(width, height);
          }
          else {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            return 102;
          }
        }
        else {
          fprintf(stderr, "Error: Vstup mimo interval!\n");
          return 101;
        }
      }
    }
  }
  else {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }
  return 0;
}

// function that prints the roof
int roof(int width) {

  for (int i = 0; i <= (width / 2) - 1; i++) {
    for (int j = 0; j < (width / 2) - i; j++) {
      printf(" ");
    }
    printf("X");
    for (int k = 0; k <= (2 * i) - 2; k++) {
      printf(" ");
    }
    if (i > 0) {
      printf("X");
    }
    printf("\n");
  }
  return 0;
}

// function that prints the walls of the house (mandatory)
int house(int width, int height) {

  for (int i = 0; i < height; i++) {
    // 1st row
    if (i == 0) {
      for (int j = 0; j < width; j++) {
        printf("X");
      }
      printf("\n");
    }
    // center
    if (i > 0 && i <= (height - 2)) {
      for (int j = 0; j < width; j++) {
        if (j == 0) {
          printf("X");
        }
        if (j > 0 && j <= (width - 2)) {
          printf(" ");
        }
        if (j == (width - 1)) {
          printf("X\n");
        }
      }
    }
    // last row
    if (i == (height - 1)) {
      for (int j = 0; j < width; j++) {
        printf("X");
      }
      printf("\n");
    }
  }
  return 0;
}

// function that prints the walls of the house (optional)
int house_with_fence(int width, int height, int fence) {

  // 1st row
  for (int j = 0; j < width; j++) {
    printf("X");
  }
  printf("\n");
  // center
  for (int i = 0; i < height - 2; i++) {
    printf("X");
    // 1st symbol
    // 'o' if the row is even
    if (i % 2 == 0) {
      for (int j = 0; j < (width - 2) / 2; j++) {
        printf("o*");
      }
      printf("oX");
      wood_fence(height, fence, i);
    }
    //'*' if the row is odd
    else {
      for (int j = 0; j < (width - 2) / 2; j++) {
        printf("*o");
      }
      printf("*X");
      wood_fence(height, fence, i);
    }
    printf("\n");
  }
  // last row
  for (int i = 0; i < width; i++) {
    printf("X");
  }
  if (fence % 2 == 0) {
    for (int j = 0; j < fence / 2; j++) {
      printf("-|");
    }
  }
  else {
    for (int j = 0; j < fence / 2; j++) {
      printf("|-");
    }
    printf("|");
  }
  printf("\n");
  return 0;
}

// function that prints the fence
int wood_fence(int height, int fence, int i) {

  // if the fence size is even
  if (fence % 2 == 0) {
    if (i == height - fence - 1) {
      for (int j = 0; j < fence / 2; j++) {
        printf("-|");
      }
    }
    if (i >= height - fence && i <= height) {
      for (int j = 0; j < fence / 2; j++) {
        printf(" |");
      }
    }
  }
  // if the size of the fence is odd
  else {
    if (i == height - fence - 1) {
      for (int j = 0; j < fence / 2; j++) {
        printf("|-");
      }
    }
    if (i >= height - fence && i <= height) {
      for (int j = 0; j < fence / 2; j++) {
        printf("| ");
      }
    }
    if (i >= height - fence - 1) {
      printf("|");
    }
  }
  return 0;
}
