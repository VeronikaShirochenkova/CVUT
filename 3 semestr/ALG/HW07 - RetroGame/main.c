#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 5000000

int max(int a, int b) {
    int c = (a > b) ? a : b;
    return c;
}

typedef struct cage {
    bool head;
    int score;
    int egg;
} cage_t;

typedef struct pos {
    int r;
    int c;
} pos_t;

int compare(const void *a, const void *b) {
  return ((pos_t*)a)->r - ((pos_t*)b)->r;
}

int main(int argc, char *argv[]) {
    int row = 0, col = 0;
    int num_of_eggs = 0;

    char ch;
    int times = 0;
    int num = 0;
    // =============== read col and num of lines ==================
    while((ch = getc_unlocked(stdin)) != '\n') {  
        if(ch >= '0' && ch <= '9') {
            num = (num == 0) ? (ch - '0') : (num * 10 + (ch - '0'));            
        }
        else {
            if (times == 0) {
                col = num;
            }
            if (times == 1) {
                num_of_eggs = num;
            }
            times++;
            num = 0;
        }
    }
    //==============================================================

    // ==================== POSITIONS ========================
    pos_t position[num_of_eggs];
    for (int i = 0; i < num_of_eggs; i++) {
        times = 0;
        while((ch = getc_unlocked(stdin)) != '\n') {  
            if(ch >= '0' && ch <= '9') {
                num = (num == 0) ? (ch - '0') : (num * 10 + (ch - '0'));            
            }
            else {
                if (times == 0) {
                    position[i].c = num - 1;
                }
                if (times == 1) {
                    position[i].r = num;
                    row = (row < position[i].r) ? position[i].r : row;
                }
                times++;
                num = 0;
            }
        }
    }

    qsort(position, num_of_eggs, sizeof(pos_t), compare);
    // =======================================================

    row++;

    int rows = 0, columns = 0;
    int count = 0;
    int maximum = -1;
    
    if ((row*col) < MAX_SIZE) {
        rows = row;
        columns = col;

        cage_t **solution = (cage_t**)malloc(sizeof(cage_t*) * rows);
        for (int i = 0; i < rows; i++) {
            solution[i] =(cage_t*) malloc(sizeof(cage_t) * columns);
        }
        
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) { 
                solution[i][j].score = 0;  
                solution[i][j].head = false;
                solution[i][j].egg = 0;
            }
        }
        solution[0][0].head = true;
        for (int i = 0; i < num_of_eggs; i++) {
            solution[position[i].r][position[i].c].egg = 1;
        }
        
        for(int r = 0; r < (rows-1); r++) {
            for(int c = 0; c < (columns - 1); c++) {
                if (solution[r][c].head == true) {
                    int front = solution[r][c].score + solution[r+1][c].egg + solution[r+1][c+1].egg;
                    int right = ((c+2) < columns) ? (solution[r][c].score + solution[r+1][c+1].egg + solution[r+1][c+2].egg) : -1;
                    int left = (c > 0) ? (solution[r][c].score + solution[r+1][c].egg + solution[r+1][c-1].egg) : -1;
                    if(c == 0) {                
                        if (solution[r+1][c].score <= front) {
                            solution[r+1][c].score = front;
                            solution[r+1][c].head = true;
                        }
                        if (solution[r+1][c+1].score <= right) {
                            solution[r+1][c+1].score = right;
                            solution[r+1][c+1].head = true;
                        } 
                    }

                    else {
                        if (solution[r+1][c-1].score <= left) {
                            solution[r+1][c-1].score = left;
                            solution[r+1][c-1].head = true;
                        }
                        if (solution[r+1][c].score <= front) {
                            solution[r+1][c].score = front;
                            solution[r+1][c].head = true;

                        }
                        if (solution[r+1][c+1].score <= right) {
                            solution[r+1][c+1].score = right;
                            solution[r+1][c+1].head = true;
                        } 
                    }
                }
                else {
                    break;
                }
                
            }
        }

        for(int i = 0;i < col; i++) {
            if(maximum < solution[row-1][i].score) {
                maximum = solution[row-1][i].score;
            }
        }    
    }
    else {

        int rws = 0;
        rws = MAX_SIZE / col;
        int ostatok = ((row % rws) > 0) ? 1 : 0;
        count = (row / rws) + ostatok;
        rows = rws;
        columns = col;
        //***********************************

        cage_t **solution = (cage_t**)malloc(sizeof(cage_t*) * rows);
        for (int i = 0; i < rows; i++) {
            solution[i] =(cage_t*) malloc(sizeof(cage_t) * columns);
        }


        int counter = 0;
        int minus = rows - 1;
        bool finish = false;

        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < columns; j++) { 
                solution[i][j].score = 0;  
                solution[i][j].head = false;
                solution[i][j].egg = 0;
            }
        }
        solution[0][0].head = true;
        
        
        NEXT:for(int i = 1; i < rows; i++) {
            for(int j = 0; j < columns; j++) { 
                solution[i][j].score = 0;  
                solution[i][j].head = false;
                solution[i][j].egg = 0;

            }
        }
        int from = minus * counter;
        for (int i = 0; i < num_of_eggs; i++) {
            if ((position[i].r - from) < rows) {
                if (counter == 0) {
                    if ((position[i].r - from) >= 0) {
                        solution[position[i].r - from][position[i].c].egg = 1;
                        if (i == (num_of_eggs - 1)) {
                            finish = true;
                        }
                    }
                }
                else if (counter > 0) {
                    if((position[i].r - from) > 0) {
                        solution[position[i].r - from][position[i].c].egg = 1;

                        if (i == (num_of_eggs - 1)) {
                            finish = true;
                        }
                    }

                }
                
                
            }
        }

        //***********************************
        

        for(int r = 0; r < (rows-1); r++) {
            for(int c = 0; c < (columns - 1); c++) {
                if (solution[r][c].head == true) {
                    int front = solution[r][c].score + solution[r+1][c].egg + solution[r+1][c+1].egg;
                    int right = ((c+2) < columns) ? (solution[r][c].score + solution[r+1][c+1].egg + solution[r+1][c+2].egg) : -1;
                    int left = (c > 0) ? (solution[r][c].score + solution[r+1][c].egg + solution[r+1][c-1].egg) : -1;
                    if(c == 0) {                
                        if (solution[r+1][c].score <= front) {
                            solution[r+1][c].score = front;
                            solution[r+1][c].head = true;
                        }
                        if (solution[r+1][c+1].score <= right) {
                            solution[r+1][c+1].score = right;
                            solution[r+1][c+1].head = true;
                        } 
                    }

                    else {
                        if (solution[r+1][c-1].score <= left) {
                            solution[r+1][c-1].score = left;
                            solution[r+1][c-1].head = true;
                        }
                        if (solution[r+1][c].score <= front) {
                            solution[r+1][c].score = front;
                            solution[r+1][c].head = true;

                        }
                        if (solution[r+1][c+1].score <= right) {
                            solution[r+1][c+1].score = right;
                            solution[r+1][c+1].head = true;
                        } 
                    }
                }
                else {
                    break;
                }
                
            }
        }
        if ((counter*minus) >= row) {
            finish = true;
        }

        counter++;
        
        for (int i = 0; i < col; i++) {
            solution[0][i].score = solution[rows-1][i].score;
            solution[0][i].head = solution[rows-1][i].head;
        }

        if (finish == false ) {
            goto NEXT;
        }
        
        for(int i = 0;i < col; i++) {
            if(maximum < solution[rows-1][i].score) {            // finding maximum among last row of dp
                maximum = solution[rows-1][i].score;
            }
        }        
    }

    //========================
    if(maximum < 10) {
            putc_unlocked((maximum + '0'), stdout);
        }
        else {
            int n = maximum;
            int count = 0;
 
            while (n / 10) {
               n = n / 10;
               count++;
            }
           
            int pow = 10;
            while(count > 1) {
               pow *= 10;
               count--;
            }
 
            while (pow) {
               putc_unlocked((maximum / pow + '0'), stdout);
               maximum = maximum % pow;
               pow = pow / 10;
            }            
        } 
    return 0;

}
