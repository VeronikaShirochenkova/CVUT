#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MORE_MEMORY 10

#define NO_END_LINE  0
#define END_OF_LINE  1
#define END_OF_FILE  2

#define FIND_PATTERN 1
#define NO_PATTERN 0

#define REG_OPTION "-E"
#define REG_OPTION_SIZE 2
#define COLOR_OPTION "--color=always"
#define COLOR_OPTION_SIZE 14

#define START_COLOR printf("\x1b[01;31m\x1b[K");
#define FINISH_COLOR printf("\x1b[m\x1b[K");

typedef struct line{
  char* pattern;
  int size_of_pattern;       
  char* string;
  int size;
  int capacity;
  int start_point;
} line_t;

// check if it's file or stdin
bool file_check(char *, int);
// open input file
void file_open(FILE** f, char* nameFile);
// close input file
void file_close(FILE** f);
// read input lines
int file_read(line_t* my_line, FILE* f);

// string initialization
void line_init(line_t* my_line);
// realloc string
void line_resize(line_t* my_line);
// free memory
void line_free(line_t* my_line);
// check if line contains a pattern
int line_find_pattern(line_t* my_line);
// check if line contains a pattern (reg)
int line_find_pattern_reg (line_t* my_line, char sign);

// get pattern size
int pattern_size(line_t*, char*);
// check which sign pattern has
char pattern_sign(char *);


int main( int argc, char *argv[] ) {
  /* GENERAL VARIABLES */
  line_t line;
  FILE *f;
  int line_counter = 0;
  int finish_read = NO_END_LINE;
  bool file_exist = file_check(argv[1], argc);

  /* OPTIONAL VARIABLES */
  int len = -1;
  bool find_start_of_pattern = false;
  int argument_size = 0;

  /* NO OPTIONS */
  if(argc == 2 || (argc == 3 && file_exist)) {

    if (file_exist) { file_open(&f, argv[2]); }
    if (!pattern_size(&line, argv[1])) { return 1; }

    while (finish_read != END_OF_FILE) {
      line_init(&line);
      finish_read = (file_exist) ? file_read(&line, f) : file_read(&line, stdin);
      
      if(line_find_pattern(&line)) {
        printf("%s\n", line.string);
        line_counter += 1;
      }
       
      line_free(&line);
    }

    if (file_exist) { file_close(&f); }
    if (line_counter == 0) { return EXIT_FAILURE; }

    return EXIT_SUCCESS;
  }
  /* OPTIONS */
  else if((argc == 3 && !file_exist) || argc == 4) {

    if (file_exist) { file_open(&f, argv[3]);}
    if (!pattern_size(&line, argv[2])) { return 1; }

    // check size of option
    char *check = argv[1];
    while (check[argument_size++]);
    argument_size--;
    char sign = '0';

    if (argument_size == REG_OPTION_SIZE) {
      sign = pattern_sign(argv[2]);
      
      if (sign == '*' || sign == '+' || sign == '?') {
        while (finish_read != END_OF_FILE) {
          line_init(&line);
          finish_read = (file_exist) ? file_read(&line, f) : file_read(&line, stdin);
          
          if(line_find_pattern_reg(&line, sign)) {
            printf("%s\n", line.string);
            line_counter += 1;
          }
          
          line_free(&line);
        }
      }
    }
      
    if (argument_size == COLOR_OPTION_SIZE) {

      while ( finish_read != END_OF_FILE ) {
        line_init(&line);
        finish_read = (file_exist) ? file_read(&line, f) : file_read(&line, stdin);

        if ( line_find_pattern(&line)) {
          for (int i = 0; i < line.size-1; i++) {

            // check if it is start of pattern
            if (line.string[i] == line.pattern[0]) {
              int j = i + 1;
              find_start_of_pattern = true;
              for (int k = 1; k < line.size_of_pattern; k++, j++) {
                if (line.string[j] != line.pattern[k]) {
                  find_start_of_pattern = false;
                  break;
                } 
              }
            }

            if (find_start_of_pattern) {
              START_COLOR;
              len = line.size_of_pattern;
              find_start_of_pattern = false;
            }

            printf("%c", line.string[i]);
            len--;

            if (len == 0 ) { FINISH_COLOR; }
          }
          printf("\n");
          line_counter += 1;
        }       
        line_free(&line);
      }
    }

    if (file_exist) { file_close(&f); }
    if (line_counter == 0 ) { return EXIT_FAILURE; }
    if (sign == '-') { return EXIT_FAILURE; }
    return EXIT_SUCCESS;    
  }
  else {
    return EXIT_FAILURE;
  }
}

bool file_check(char *check, int num_of_arg) {
  int counter = 0;
  int count = 0;

  if (num_of_arg < 3) { return false; } 
  if (num_of_arg == 4) { return true; }

  if (num_of_arg == 3) {
    while (check[counter++]);
    counter--;
    /* -E */
    if (counter == REG_OPTION_SIZE) {
      char *reg = REG_OPTION;

      for (int i = 0; i < REG_OPTION_SIZE; i++) {
        if (check[i] == reg[i]) {
          count++;
        }
        else {
          break;
        } 
      }
      
      if (count == REG_OPTION_SIZE) { return false; }      
    }
    /* --color=always */
    else if (counter == COLOR_OPTION_SIZE) {
      char *color = COLOR_OPTION;

      for (int i = 0; i < COLOR_OPTION_SIZE; i++) {
        if (check[i]==color[i]) {
          count++;
        }
        else {
          break;
        } 
      }

      if (count == COLOR_OPTION_SIZE) { return false; }      
    }
  }
  return true;
}
 
void file_open ( FILE** f, char* nameFile ) {
  *f = fopen(nameFile, "r");
  if ( *f == NULL ) {
    exit(1);
  }
}

void file_close ( FILE** f ) {
  if ( fclose(*f) == EOF ) {
    exit(1);
  }
}

int file_read(line_t* my_line, FILE* f) {

  int  finish_line = NO_END_LINE;

  while (!finish_line) {
    char ch = fgetc(f);
    if (my_line->size + 2 >= my_line->capacity ) {
      line_resize(my_line);
    }

    if (ch == '\n') {
      finish_line = END_OF_LINE;
      break;
    }

    if (ch == EOF) {
      finish_line = END_OF_FILE;
      break;
    }

    my_line->string[my_line->size++] = ch;
  }

  my_line->string[my_line->size++] = '\0';
  return finish_line;
}

void line_init ( line_t* my_line ) {
  my_line->string = (char*)malloc(sizeof(char));
  my_line->size = 0;
  my_line->capacity = 1;
  my_line->start_point = 0;
  my_line->string[0] = '\0';
}
 
void line_resize ( line_t* my_line ) {
  my_line->capacity += MORE_MEMORY;
  my_line->string = realloc(my_line->string, my_line->capacity * sizeof(char));
}
 
void line_free(line_t* my_line) {
  if (my_line->string != NULL) {
    free(my_line->string);
  }
}
 
int line_find_pattern (line_t* my_line) {

  int length = my_line->size_of_pattern;
  int i = 0;
  while ( my_line->string[i] ) {
    if ( my_line->string[i] == my_line->pattern[0] ) {
      int j = 1;
      while ( j < length && i + j <  my_line -> size && my_line->string[i + j] == my_line->pattern[j] ) {
        j++;
      }
      if ( j == length ) { 
        return FIND_PATTERN;
      }
    }
    i++;
  }
  return NO_PATTERN;
}

int line_find_pattern_reg (line_t* my_line, char sign) {

  int i = 0;
  while ( my_line->string[i] ) {
    if ( my_line->string[i] == my_line->pattern[0]) {

      int l = i;
      int p = 0;

      while(my_line->string[l] == my_line->pattern[p]) {
        l++;
        p++;
      }

      if (my_line->pattern[p+1] == sign && sign != '+') {
        p += 2;
        
        if(my_line->string[l] == my_line->pattern[p])  {
          while(p < my_line->size_of_pattern && my_line->string[l] == my_line->pattern[p] ) {
            l++;
            p++;
          }
          if (p == my_line->size_of_pattern) { return FIND_PATTERN; }
        } 
      } 
      else if (my_line->pattern[p] == sign) {
        char ch = my_line->pattern[p-1];
        p++;
        if (my_line->string[l] == ch) {
          while (sign != '?' && my_line->string[l] == ch) { l++; } 
          if (my_line->string[l] == my_line->pattern[p]) { return FIND_PATTERN; }  
        }
        else if (my_line->string[l] == my_line->pattern[p]) {
          
          while(p < my_line->size_of_pattern && my_line->string[l] == my_line->pattern[p] ) {
            l++;
            p++;
          }
          if (p == my_line->size_of_pattern) { return FIND_PATTERN; }
        }
      }     
    }
    i++;
  }
  return NO_PATTERN;
}

int pattern_size(line_t* my_line, char* pttrn) {
  int length = 0;
  my_line->pattern = pttrn;

  while(my_line->pattern[length]) {
    length++;
  }
  my_line->size_of_pattern = length;

  if (!length) {
    return NO_PATTERN;
  }
  else {
    return FIND_PATTERN;
  }
}

char pattern_sign(char *pattern) {
  int i = 0;
  char ch = '-';
  while(pattern[i++]) {
    if (pattern[i] == '?') {
      ch = pattern[i];
      return ch;
    }
    if (pattern[i] == '*') {
      ch = pattern[i];
      return ch;
    }
    if (pattern[i] == '+') {
      ch = pattern[i];
      return ch;
    }
  }
  return ch;
}
