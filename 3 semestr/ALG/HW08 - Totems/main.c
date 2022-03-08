#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define DIFF(x, y) ((x) >= (y) ? (x - y) : (y - x))

typedef struct block {
    int villagers;
    int profit;
} block_t;


/* MAIN */
int main(int argc, char *argv[]) {

    int num_of_village = 0;
    int totem_price = 0;
    int warrior_price = 0;

    scanf("%d %d %d", &num_of_village, &totem_price, &warrior_price);

    int villages[num_of_village];
    for (int i = 0; i < num_of_village; i++) {
        scanf("%d", &villages[i]);
    }

    block_t **arr = (block_t **)malloc(sizeof(block_t *) * num_of_village);
    for (int i = 0; i < num_of_village; i++) {
        arr[i] = (block_t *)malloc(sizeof(block_t) * num_of_village);
    }

    for (int r = 0; r < num_of_village; r++) {
        for (int c = 0; c < num_of_village; c++) {
            if (r == c) {
                arr[r][c].villagers = villages[r];
                arr[r][c].profit = 0;
            }
            else {
                arr[r][c].villagers = 0;
                arr[r][c].profit = -1000000;
            }
        }
    }

    for (int r = 0; r < (num_of_village - 1); r++) {
        arr[r][r + 1].villagers = arr[r][r].villagers + arr[r + 1][r + 1].villagers;
        arr[r][r + 1].profit = totem_price - (DIFF(arr[r][r].villagers, arr[r + 1][r + 1].villagers)) * warrior_price;
    }

    // 1ST CYCLE
    for (int n = 2; n < num_of_village; n++) {
        int to = num_of_village - n;
        for (int i = 0; i < to; i++) {
            int times = DIFF(i, (i + n));
            int profit = 0;
            int a = i;
            for (int idx = 0; idx < times; idx++) {
                profit = totem_price - (DIFF(arr[i][a].villagers, arr[a + 1][i + n].villagers)) * warrior_price + arr[i][a].profit + arr[a + 1][i + n].profit;
                if (profit > arr[i][i + n].profit) {
                    arr[i][i + n].profit = profit;
                    arr[i][i + n].villagers = arr[i][a].villagers + arr[a + 1][i + n].villagers;
                }
                a++;
            }
        }
    }

    // 2ND CYCLE
    for (int n = 2; n < num_of_village; n++) {
        int to = num_of_village - n;
        for (int i = 0; i < to; i++) {
            int times = DIFF(i, (i + n));
            int profit = 0;
            int a = i;
            for (int idx = 0; idx < times; idx++) {
                profit = arr[i][a].profit + arr[a + 1][i + n].profit;
                if (profit > arr[i][i + n].profit) {
                    arr[i][i + n].profit = profit;
                    arr[i][i + n].villagers = arr[i][a].villagers + arr[a + 1][i + n].villagers;
                }
                a++;
            }
        }
    }

    // RESULT
    printf("%d\n", arr[0][num_of_village - 1].profit);
    return 0;
}
