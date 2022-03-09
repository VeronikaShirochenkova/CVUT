#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_SIZE  1000000
#define MAX_TEST 20

typedef struct cell {
    int price;
    int not_free;
    bool use;
} cell_t;

typedef struct array {
    int num;
    int distance;
    int row;
    int col;
    cell_t **cells;
    int *r;
    int *c;
} array_t;

int result = INIT_SIZE;
int res_counter = 0;

/* function that allocates memory for cells array */
void array_alloc(array_t*);
/* function that reads input data */
void array_read(array_t *);
/* function that frees all allocated memory */
void array_free(array_t *);
/* function that changes status of cells */
void array_change(array_t *, int, int);
/* function that changes back status of cells */
void array_change_back(array_t *, int, int);
/* function that searchs minimum price */
void search(array_t *, int, int);

/* MAIN */
int main(int argc, char *argv[]) {
    /* INITIALIZATION */
    array_t arr;
    scanf("%d %d %d %d", &arr.row, &arr.col, &arr.distance, &arr.num);
    array_alloc(&arr);
    array_read(&arr);
    /* START */
    search(&arr, 0, 0);
    /* FINISH */
    printf("%d\n", result);
    array_free(&arr);
    return 0;
}

void array_alloc(array_t *array) {
    array->cells = (cell_t**)malloc(sizeof(cell_t*)*array->row);
    for(int i = 0; i < array->row; i++) {
        array->cells[i]=(cell_t*)malloc(sizeof(cell_t)*array->col);
    }
    array->r = malloc(sizeof(int) * array->row);
    array->c = malloc(sizeof(int) * array->col);

}

void array_read(array_t *array) {
    for (int i = 0; i < array->row; i++) {
        for (int j = 0; j < array->col; j++) {
            scanf("%d", &array->cells[i][j].price);
            array->cells[i][j].not_free = 0;
            array->cells[i][j].use = false;
        }    
    }
    for (int i = 0; i < array->row; i++) {
        array->r[i] = 0;
    }
    for (int i = 0; i < array->col; i++)
    {
        array->c[i] = 0;
    }  
}

void array_free(array_t *array) {
    for(int i = 0; i < array->row; i++) {
       free(array->cells[i]);
    }
    free(array->cells);

    free(array->r);
    free(array->c);
}

void array_change(array_t *arr, int r, int c) {
    arr->r[r] = 1;
    arr->c[c] = 1;
    arr->num -= 1;
    /* LEFT UPPER CORNER */
    if (r == 0 && c == 0) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance - i); j++) {
                arr->cells[i][j].not_free += 1;
            }         
        }
        
    }
    /* LEFT LOWER CORNER */
    else if (r == (arr->row-1) && c == 0) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                arr->cells[r-j][i].not_free += 1;
            }
        }
        
    }
    /* RIGHT UPPER CORNER */
    else if (r == 0 && c == (arr->col-1)) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                arr->cells[i][c-j].not_free += 1;
            }
        }
    }
    /* RIGHT LOWER CORNER */
    else if (r == (arr->row-1) && c == (arr->col-1)) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                arr->cells[r-j][c-i].not_free += 1;
            }
        }
    }
    /* NOT CORNER */
    else {
        // left up
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                if ((r-j) >= 0 && (c-i) >= 0) {
                    arr->cells[r-j][c-i].not_free += 1;
                }      
            } 
        }
        // right up
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                if ((r-j) >= 0 && (c+i) < arr->col) {
                    arr->cells[r-j][c+i].not_free += 1;
                }       
            }
        }
        // left down
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                if ((r+i)< arr->row && (c-j) >= 0) {
                    arr->cells[r+i][c-j].not_free += 1;  
                } 
            }
        }
        // right down
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 1; j < (arr->distance - i); j++) {
                if ((r+i) < arr->row && (c+j) < arr->col) {
                    arr->cells[r+i][c+j].not_free += 1;
                } 
            }         
        }
    }
}

void array_change_back(array_t *arr, int r, int c) {
    arr->r[r] = 0;
    arr->c[c] = 0;
    arr->num += 1;
    /* LEFT UPPER CORNER */
    if (r == 0 && c == 0) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance - i); j++) {
                arr->cells[i][j].not_free -= 1;
            }         
        }
        
    }
    /* LEFT LOWER CORNER */
    else if (r == (arr->row-1) && c == 0) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                arr->cells[r-j][i].not_free -= 1;
            }
        }
        
    }
    /* RIGHT UPPER CORNER */
    else if (r == 0 && c == (arr->col-1)) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                arr->cells[i][c-j].not_free -= 1;
            }
        }
    }
    /* RIGHT LOWER CORNER */
    else if (r == (arr->row-1) && c == (arr->col-1)) {
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                arr->cells[r-j][c-i].not_free -= 1;
            }
        }
    }
    /* NOT CORNER */
    else {
        // left up
        for (int i = 0; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                if ((r-j) >= 0 && (c-i) >= 0) {
                    arr->cells[r-j][c-i].not_free -= 1;
                }      
            } 
        }
        // right up
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) {
                if ((r-j) >= 0 && (c+i) < arr->col) {
                    arr->cells[r-j][c+i].not_free -= 1;
                }       
            }
        }
        // left down
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 0; j < (arr->distance-i); j++) { 
                if ((r+i)< arr->row && (c-j) >= 0) {
                    arr->cells[r+i][c-j].not_free -= 1;  
                } 
            }
        }
        // right down
        for (int i = 1; i < arr->distance; i++) {
            for (int j = 1; j < (arr->distance - i); j++) {
                if ((r+i) < arr->row && (c+j) < arr->col) {
                    arr->cells[r+i][c+j].not_free -= 1;
                } 
            }         
        }
    }
}

void search(array_t *arr, int total, int sr) {
    if (res_counter < MAX_TEST) {
        if (arr->num <= 0) {
            if (result == total) { res_counter++; }
            result = (total < result) ? total : result;
        }
        else {  
            for (int r = sr; r < arr->row; r++) {
                if (arr->r[r] == 0 ) {
                    for (int c = 0; c < arr->col; c++) {
                        if (arr->c[c] == 0 && arr->cells[r][c].not_free == 0) {  
                            //change                  
                            arr->cells[r][c].use = true;    
                            array_change(arr, r, c);
                            // go search
                            search(arr, (total+arr->cells[r][c].price), (r+1));                            
                            // change back
                            array_change_back(arr, r, c);
                            arr->cells[r][c].use = false;
                        }
                        
                    }
                }
                
            }
        }
    }  
}
