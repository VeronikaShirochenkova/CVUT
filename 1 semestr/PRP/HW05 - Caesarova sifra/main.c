#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
 
// length of [a-z] or [A-Z]
#define ENG 26
// length of [a-zA-Z]
#define LEN_AlPHA 52
// first length 
#define FIRST_LEN 10
// minimum of two numbers
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* 
 * tuto funkci jsem prevzala z videa:
 * https://cw.fel.cvut.cz/wiki/courses/b0b36prp/resources/records/lec06 
 * function which reads the input string
 */
char* read_line(int* line_length);
// search length of shift 
int search_shift(char*, char*, int);
// decode without errors
int good_decoding(char*, int, int);
// search the Levenshtein distance
int levenshtein(char*, char*, int, int);
// search length of shift (optional)
int search_optional(char*, char*, int, int);
 
int main(int argc, char* argv[]) {
 
    int shift = 0;
    char* code, * bad_decode;
    int code_length, bad_decode_length;
 
    code = NULL, code_length = 0;
    bad_decode = NULL, bad_decode_length = 0;
 
    code = read_line(&code_length);
    if (code) {
        bad_decode = read_line(&bad_decode_length);
    }
    // if code contains incorrect characters
    for (int i = 0; i < code_length; i++) {
        if (isalpha(code[i]) == 0) {
            free(code);
            free(bad_decode);
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
    }
    // if decode contains incorrect characters
    for (int i = 0; i < bad_decode_length; i++) {
        if (isalpha(bad_decode[i]) == 0) {
            free(code);
            free(bad_decode);
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
    }
    // if it's optional task
    if (argc == 2) {
        search_optional(code, bad_decode, code_length, bad_decode_length);
    }
    // if it's compulsory task 
    else {
        // if the two lengths are different  
        if (code_length != bad_decode_length) {
            free(code);
            free(bad_decode);
            fprintf(stderr, "Error: Chybna delka vstupu!\n");
            return 101; 
        }
        // if the two lengths are the same
        else {
            shift = search_shift(code, bad_decode, code_length);
            good_decoding(code, shift, code_length);
        }
    }

    free(code);
    free(bad_decode);
    return 0;
}
 
char* read_line(int* line_length) {
 
    int capacity = FIRST_LEN;
    char* line = malloc(capacity + 1);
    int length = 0;
 
    if (line == NULL) {
        fprintf(stderr, "Error: Chybny vstup!\n");
    }
    else {
        int letter = 0;
        while ((letter = getchar()) != EOF && letter != '\n') {
            if (length == capacity) {
                char* line_v2 = realloc(line, capacity * 2);
                if (line_v2 == NULL) {
                    free(line);
                    line = NULL;
                    length = 0;
                    break;
                }
                capacity *= 2;
                line = line_v2;
            }
            line[length++] = letter;
        }
    }
    *line_length = length;
    return line;
}
 
int search_shift(char* code, char* bad_decode, int code_length) {
    
    int counter = 0, minimum = 0, shift = 0;
 
    for (int i = 0; i < LEN_AlPHA; i++) {
        for (int j = 0; j < code_length; j++) {
            if (code[j] == 'Z') {
                code[j] = 'z';
                code[j] = code[j] - ENG;
            }
            if (code[j] == 'z') {
                code[j] = 'Z';
                code[j] = code[j] - ENG;
            }
            if ((code[j] != 'Z') || (code[j] != 'z')) {
                code[j] = code[j] + 1;
            }
        }
 
        for (int k = 0; k < code_length; k++) {
            if (code[k] == bad_decode[k]) {
                counter += 1;
            }
        }
        if (counter > minimum) {
            minimum = counter;
            shift = i + 1;
        }
        counter = 0;
    }
    return shift;
}

int search_optional(char* code, char* bad_decode, int code_length, int bad_decode_length) {
    int res1 = 0, res2 = 0; 
    char result [code_length];
    
    for (int a = 0; a < LEN_AlPHA; a++) {
        for (int b = 0; b < code_length; b++) {
            if (code[b] == 'Z') {
                code[b] = 'z';
                code[b] = code[b] - ENG;
            }
            if (code[b] == 'z') {
                code[b] = 'Z';
                code[b] = code[b] - ENG;
            }
            if ((code[b] != 'Z') || (code[b] != 'z')) {
                code[b] = code[b] + 1;
            }
        }
        // write 1st result
        if (a == 0) {
            for (int c = 0; c < code_length; c++) {
                result[c] = code[c];
            }
            res1 = levenshtein(code, bad_decode, code_length, bad_decode_length);
        }
        else {
            res2 = levenshtein(code, bad_decode, code_length, bad_decode_length);
            // if the new length is shorter - rewrite 
            if (res2 < res1) {
                res1 = res2;
                for (int i = 0; i < code_length; i++) {
                    result[i] = code[i];
                }
            }
        }
    }
    for (int j = 0; j < code_length; j++) {
        printf("%c", result[j]);
    }
    printf("\n");
    return 0;

}
 
int good_decoding(char* code, int shift, int code_length) {

    for (int i = 0; i < shift; i++) {
        for (int j = 0; j < code_length; j++) {
            if (code[j] == 'Z') {
                code[j] = 'z';
                code[j] = code[j] - ENG;
            }
            if (code[j] == 'z') {
                code[j] = 'Z';
                code[j] = code[j] - ENG;
            }
            if ((code[j] != 'Z') || (code[j] != 'z')) {
                code[j] = code[j] + 1;
            }
        }
    }
    for (int k = 0; k < code_length; k++) {
        printf("%c", code[k]);
    }
    printf("\n");
    return 0;
}

int levenshtein (char* code, char* decode, int code_l, int bad_decode_l) {
 
    int code_length = code_l + 1;
    int bad_decode_length = bad_decode_l + 1;

    int capacity = code_length * bad_decode_length;
    int *array = malloc(sizeof(int)*capacity);
    int len = 0;

    // fill 1st row 
    for (int i = 0; i < code_length; i++) {
        array[i] = i;
    }
    
    // fill 1st col
    for (int j = 0; j < capacity; j+=code_length) {
        array[j] = j / code_length ;
    }

   /* 
    * fill with 1 and 0
    * 0 - letters are same,
    * 1 - letters are different
    */
    int a, b;
    for( int i = code_length; i < capacity; i++) {
        if ((i % code_length) > 0 ) {
            a = i % code_length;
            b = i / code_length;
            if (code[a - 1] == decode[b - 1]) {
                array[i] = 0;
            }
            else {
                array[i] = 1;
            }
        }
        
    }

    // L. distance calculation
    int x,y,z;
    for (int i = code_length; i < capacity; i++) {
        if (array[i] == 1 && i > code_length) {
            x = array[i - 1] + 1;
            y = array[i - code_length - 1] + 1;
            z = array[i - code_length] + 1;
            array[i] = MIN(MIN(x,y),z);
        }
        if (array[i] == 0 && i > code_length) {
            x = array[i - 1] + 1;
            y = array[i - code_length - 1] + 0;
            z = array[i - code_length] + 1;
            array[i] = MIN(MIN(x,y),z);
        }
    }
    len = array[capacity - 1];
    free(array);
    return len;
}
