#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <set>
#include <string>

/* FIELD */
typedef struct cell {
    int price;
    bool free;
    int hranice;
}cell_t;

typedef struct field {
    int rows;
    int columns;
    cell_t **cells;
}field_t;

/* POZEMKY */
typedef struct area {
    int row;
    int col;
    int square;         // plocha daneho pozemku
    int pocet_zajemcu;  // pocet pozemku daneho typu
}area_t;

typedef struct types {
    int count;
    area_t *areas;
}types_t;


int global_square = 0;

void memory_allocf(field_t*);
void array_readf(field_t*);

void memory_alloct(types_t*);
void array_readt(types_t*);

void array_print(field_t*);

void memory_freef(field_t);
void memory_freet(types_t);

int compare(const void *, const void *);

void search(field_t*, types_t*,std::set<std::string>&);
bool valid(field_t*, area_t*, int, int);
void try_field_change(field_t*, area_t*, int, int);
void back_field_change(field_t*, area_t*, int, int);
int square(field_t*);
void hash_field(field_t*, std::string&);

int main(int argc, char *argv[]) {

    /* FIELD */
    field_t fld;
    scanf("%d %d", &fld.rows, &fld.columns); 
    memory_allocf(&fld);
    array_readf(&fld);
    std::set<std::string> memory;

    /* LAND TYPES */
    types_t types;
    scanf("%d ", &types.count);
    memory_alloct(&types);
    array_readt(&types);
    qsort(types.areas, types.count, sizeof(area_t), compare);

    /* HASH ARRAY */


    /* SEARCH */
    search(&fld, &types, memory);
    printf("%d\n", global_square);

    /* MEMORY FREE */
    memory_freef(fld);
    memory_freet(types);

    return 0;
}

void memory_allocf(field_t* array) {

    array->cells = (cell_t**)malloc(sizeof(cell_t*)*array->rows);
    for(int i = 0; i < array->rows; i++) {
        array->cells[i]=(cell_t*)malloc(sizeof(cell_t)*array->columns);
    }
}

void array_readf(field_t *array) {

    for (int i = 0; i < array->rows; i++) {
        for (int j = 0; j < array->columns; j++) {
            scanf("%d ", &array->cells[i][j].price);
            array->cells[i][j].free = true;
            array->cells[i][j].hranice = 0;
        }
    }
}

void memory_alloct(types_t *array) {
    array->areas = (area_t*)malloc(sizeof(area_t)*array->count);
}

void array_readt(types_t *array) {
    for (int i = 0; i < array->count; i++) {
        scanf("%d %d %d ", &array->areas[i].row, &array->areas[i].col, &array->areas[i].pocet_zajemcu);
        array->areas[i].square = array->areas[i].row * array->areas[i].col;
    }
}

void array_print(field_t* array) {

    for (int i = 0; i < array->rows; i++) {
        for (int j = 0; j < array->columns; j++) {
            if (array->cells[i][j].free || array->cells[i][j].hranice) {
                printf("X ");
            }
            else {
                printf("0 ");
            }        
        }
        printf("\n");
    }
    
}

void memory_freef(field_t array) {

    for(int i = 0; i < array.rows; i++) {
       free(array.cells[i]);
    }
    free(array.cells);
}

void memory_freet(types_t array) {
    free(array.areas);
}

int compare(const void *a, const void *b) {
    return ((area_t *)b)->square - ((area_t *)a)->square;
}

void search (field_t* field, types_t* types, std::set<std::string>& memory) {

    bool at_least_one_added = false;

    for (int r = 0; r < field->rows; r++ ) {
        for(int c = 0; c < field->columns; c++ ) {
            if (field->cells[r][c].free) {
                if ((r > 0) && (field->cells[r-1][c].free)) { //|| ((c > 0) && (field->cells[r][c-1].free))
                    /* means higher is a better position */
                    break;
                } else {
                    for (int i = 0; i < types->count; i++) {
                        if (types->areas[i].pocet_zajemcu) {
                            if (valid(field, &types->areas[i], r, c)) {
                                types->areas[i].pocet_zajemcu--; 
                                try_field_change(field, &types->areas[i], r, c);
                                std::string str;
                                hash_field(field, str);
                                if (memory.find(str) == memory.end()) {
                                    memory.insert(str);
                                    search(field, types, memory);
                                }
                                // if hash doesn't exist go next
                                back_field_change(field, &types->areas[i], r, c);
                                types->areas[i].pocet_zajemcu++;
                                at_least_one_added = true;
                            }
                        }
                    }
                    break;
                }
            }
        }   
    }
    /* nothing else can be placed */
    if (!at_least_one_added) {
        int sq = square(field);
        if (sq > global_square) {
            global_square = sq;
        }
    }

}

bool valid(field_t* field, area_t* area, int r, int c) {
    /* if area fits in height and width */
    if ( ((r + area->row) <= field->rows) && ((c + area->col) <= field->columns )) {
        /* if all cells are free */
        for (int i = r; i < (r + area->row); i++) {
            for (int j = c; j < (c + area->col); j++) {
                if (field->cells[i][j].free == false) {
                    return false;
                }   
            }
        }
        return true;
    }
    else {
        return false;
    }
}

void try_field_change(field_t* field, area_t* area, int r, int c) {
    // INSIDE
    for (int a = r; a < (r + area->row); a++) {
        for (int b = c; b < (c + area->col); b++) {
            field->cells[a][b].free = false;
        }
    }
    // UP
    if(r > 0)  {
        for (int c_up = c; c_up < (c + area->col); c_up++) {
            field->cells[r-1][c_up].free = false;
            field->cells[r-1][c_up].hranice += 1;                              
        }
        // UPPER LEFT CORNER
        if(c > 0) {
            field->cells[r-1][c-1].free = false;
            field->cells[r-1][c-1].hranice += 1;
        }
        // UPPER RIGHT CORNER
        if(c + area->col < field->columns) {
            field->cells[r-1][c+area->col].free = false;
            field->cells[r-1][c+area->col].hranice += 1;
        }
    }
    // DOWN
    if( r + area->row < field->rows ) {
        for (int c_down = c; c_down < (c + area->col); c_down++) { 
            field->cells[r+area->row][c_down].free = false;
            field->cells[r+area->row][c_down].hranice +=1;     
        }
        // BOTTOM LEFT CORNER
        if( c > 0 ) {
            field->cells[r+area->row][c-1].free = false;
            field->cells[r+area->row][c-1].hranice +=1;
        }
        // BOTTOM RIGHT CORNER
        if( c + area->col < field->columns ) {
            field->cells[r+area->row][c+area->col].free = false;
            field->cells[r+area->row][c+area->col].hranice += 1;
        }
    }
    // LEFT
    if( c > 0 ) {
        for (int r_left = r; r_left < (r + area->row); r_left++) {
            field->cells[r_left][c-1].free = false;
            field->cells[r_left][c-1].hranice += 1;
        }
    }
    // RIGHT
    if( c + area->col < field->columns ) {
        for (int r_right = r; r_right < (r + area->row); r_right++) { 
            field->cells[r_right][c+area->col].free = false;
            field->cells[r_right][c+area->col].hranice += 1;
        }
    }
}

void back_field_change(field_t* field, area_t* area, int r, int c) {
    // INSIDE
    for (int a = r; a < (r + area->row); a++) {
        for (int b = c; b < (c + area->col); b++) {
            field->cells[a][b].free = true;
        }
    }
    // UP
    if(r > 0)  {
        for (int c_up = c; c_up < (c + area->col); c_up++) {
            field->cells[r-1][c_up].hranice -= 1;
            if (field->cells[r-1][c_up].hranice == 0) {field->cells[r-1][c_up].free = true;}                                
        }
        // UPPER LEFT CORNER
        if(c > 0) {
            field->cells[r-1][c-1].hranice -= 1;
            if (field->cells[r-1][c-1].hranice == 0) {field->cells[r-1][c-1].free = true;}
        }
        // UPPER RIGHT CORNER
        if(c + area->col < field->columns) {
            field->cells[r-1][c+area->col].hranice -= 1;
            if (field->cells[r-1][c+area->col].hranice == 0) {field->cells[r-1][c+area->col].free = true;}
        }
    }
    // DOWN
    if( r + area->row < field->rows ) {
        for (int c_down = c; c_down < (c + area->col); c_down++) { 
            field->cells[r+area->row][c_down].hranice -= 1;    
            if (field->cells[r+area->row][c_down].hranice == 0) {field->cells[r+area->row][c_down].free = true;}
        }
        // BOTTOM LEFT CORNER
        if( c > 0 ) {
            field->cells[r+area->row][c-1].hranice -= 1;
            if (field->cells[r+area->row][c-1].hranice == 0) {field->cells[r+area->row][c-1].free = true;}
        }
        // BOTTOM RIGHT CORNER
        if( c + area->col < field->columns ) {
            field->cells[r+area->row][c+area->col].hranice -= 1;
            if (field->cells[r+area->row][c+area->col].hranice == 0) {field->cells[r+area->row][c+area->col].free = true;}
        }
    }
    // LEFT
    if( c > 0 ) {
        for (int r_left = r; r_left < (r + area->row); r_left++) {
            field->cells[r_left][c-1].hranice -= 1;
            if (field->cells[r_left][c-1].hranice == 0) {field->cells[r_left][c-1].free = true;}
        }
    }
    // RIGHT
    if( c + area->col < field->columns ) {
        for (int r_right = r; r_right < (r + area->row); r_right++) { 
            field->cells[r_right][c+area->col].hranice -= 1;
            if (field->cells[r_right][c+area->col].hranice == 0) {field->cells[r_right][c+area->col].free = true;}
        }
    }
}

int square(field_t* field) {

    int local_square = 0;
    for (int r = 0; r < field->rows; r++) {
        for (int c = 0; c < field->columns; c++) {
            if (field->cells[r][c].free==false && field->cells[r][c].hranice == 0) {
                local_square += field->cells[r][c].price;
            }
        }
    }  
    return local_square;

}

void hash_field(field_t* field, std::string& str) {

    for (int r = 0; r < field->rows; r++) {
        for (int c = 0; c < field->columns; c++) {
            str.push_back(field->cells[r][c].hranice - 48);
            // hash += 3*hash + field->cells[r][c].hranice;
            // printf("%d ", hash);
        }
    }  
}
