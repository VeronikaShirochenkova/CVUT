#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1000000
#define MIN -1000000

typedef struct nbr {
    struct node_t *neighbr;
}nbr_t;

typedef struct node_t {
    int num;
    int D;                      // hodnota nasycení karavany
    int p;                      // min pocet baliku pridavnych zasob    
    bool friendly;
    bool visited;
    bool in; 
    int count;
    struct node_t **neighbors;                    
}node_t;

typedef struct graph {
    int num_of_nodes;
    int count_not_visited;
    int num_friendly_village;
    int fullness;
    int start_point;
    int global_p;
    node_t *nodes;
}graph_t;

/* allocate memory */
void alloc_graph(graph_t *);
/* free allocated memory */
void free_memory(graph_t *);
/* variable initialization */
void create_graph(graph_t *);
/* reading input data */
void read_graph(graph_t *, int);
/* calculating 'D' and 'p' for neighboring nodes */
void counting_values(graph_t *, int);
/* write nodes to array and making calculation for each node */
void search(graph_t *);
/* print final result */
void final_result(graph_t *);

int main(int argc, char *argv[])  {

    /* CREATE & ALLOCATE */
    graph_t graph;
    int roads = 0;
    scanf("%d %d %d %d", &graph.num_of_nodes, &roads, &graph.num_friendly_village, &graph.fullness);
    alloc_graph(&graph);
    create_graph(&graph);
    read_graph(&graph, roads);

    /* DO */
    search(&graph);
    final_result(&graph);

    /* FREE */
    free_memory(&graph);

    return 0;
}

void alloc_graph(graph_t *grph) {
    
    grph->nodes = (node_t*)malloc(sizeof(node_t) * (grph->num_of_nodes + 1));
    for (int i = 1; i <= grph->num_of_nodes; i++) {
        grph->nodes[i].neighbors = (node_t**)malloc(sizeof(node_t*)* (grph->num_of_nodes-1));
    }
    
}

void free_memory(graph_t *grph) {

    for (int i = 1; i <= grph->num_of_nodes; i++) {
        free(grph->nodes[i].neighbors);
    }
    free(grph->nodes);
    
}

void create_graph(graph_t *grph) {

    grph->global_p = 0;
    grph->start_point = 1;
    grph->count_not_visited = grph->num_of_nodes;

    for (int i = 1; i <= grph->num_of_nodes; i++) {
        grph->nodes[i].num = i;

        if (i <= grph->num_friendly_village) {
            grph->nodes[i].friendly = true;
        }
        else {
            grph->nodes[i].friendly = false;
        }

        grph->nodes[i].count = 0;
        grph->nodes[i].visited = false;
        grph->nodes[i].in = false;
        
        if (i == grph->start_point) {
            grph->nodes[i].D = grph->fullness;
            grph->nodes[i].p = 0;
        }
        else {
            grph->nodes[i].D = MIN;
            grph->nodes[i].p = MAX;
        }     
    }   
}

void read_graph(graph_t *grph, int num_of_roads) {

    int a = 0, b = 0;
    for (int i = 0; i < num_of_roads; i++) {
        scanf("%d %d", &a, &b);
        int a1 = grph->nodes[a].count;
        int b1 = grph->nodes[b].count;

        grph->nodes[a].neighbors[a1] = &grph->nodes[b];
        grph->nodes[a].count ++;

        grph->nodes[b].neighbors[b1] = &grph->nodes[a];
        grph->nodes[b].count ++;
    }
    
}

void counting_values (graph_t *grph, int start) {

    node_t *village = &grph->nodes[start];

    for (int i = 0; i < village->count; i++) {
        // if neighboring village is friendly 
        if (village->neighbors[i]->friendly) {
            village->neighbors[i]->D = grph->fullness;

            if (village->D == 0 && (village->neighbors[i]->p > (village->p + 1))) {
                village->neighbors[i]->p = village->p + 1;

                if (village->neighbors[i]->visited ) {
                    village->neighbors[i]->visited = false;
                    grph->count_not_visited++;
                }

            }
            if (village->D > 0 && (village->neighbors[i]->p > village->p)){
                village->neighbors[i]->p = village->p;

                if (village->neighbors[i]->visited ) {
                    village->neighbors[i]->visited = false;
                    grph->count_not_visited++;
                }

            }
        } 
        // if neighboring village is neutral
        else {
            if (village->D > 0){
                if(((village->D-1) > village->neighbors[i]->D && village->p == village->neighbors[i]->p) || 
                    (village->p < village->neighbors[i]->p)) {

                    village->neighbors[i]->D = village->D - 1;
                    village->neighbors[i]->p = village->p;

                    if (village->neighbors[i]->visited ) {
                        village->neighbors[i]->visited = false;
                        grph->count_not_visited++;
                    }

                }
            }
            else {
                if((grph->fullness > village->neighbors[i]->D && (village->p + 1) == village->neighbors[i]->p) || 
                   ((village->p + 1) < village->neighbors[i]->p) ) {

                    village->neighbors[i]->D = grph->fullness;
                    village->neighbors[i]->p = village->p + 1;

                    if (village->neighbors[i]->visited ) {
                        village->neighbors[i]->visited = false;
                        grph->count_not_visited++;
                    }

                }
            }
        }
    }

    village->visited = true;
    grph->count_not_visited--;
}

void search(graph_t *grph) {

    // create array of graph nodes
    int array[grph->num_of_nodes];

    for (int i = 0; i < grph->num_of_nodes; i++) {
        array[i] = 0;
    } 
    array[0] = grph->nodes[1].num;
    grph->nodes[1].in = true;
    int head = 0, pointer = 0, fix = 1;

    while (head < grph->num_of_nodes) { 

        pointer = array[head];
        for (int i = 0; i < grph->nodes[pointer].count; i++) {
            if (!grph->nodes[pointer].neighbors[i]->visited && !grph->nodes[pointer].neighbors[i]->in) {
                array[fix] = grph->nodes[pointer].neighbors[i]->num; 
                grph->nodes[pointer].neighbors[i]->in = true;
                fix ++;
            }
        }
        grph->nodes[pointer].visited = true;
        head++;

    }

    for (int i = 1; i <= grph->num_of_nodes; i++) {
        grph->nodes[i].visited = false;
    }


    while ( grph->count_not_visited != 0 ) {

        for (int i = 0; i < grph->num_of_nodes; i++) {
            if (!grph->nodes[array[i]].visited) {
               counting_values(grph, array[i]); 
            }        
        }

    }
            
}

void final_result(graph_t *grph) {
    int min = 0, max = 0;

    for (int i = 1; i <= grph->num_of_nodes; i++) {
        if (grph->nodes[i].p == 0) {
            min ++;
        }
        if (grph->nodes[i].p > max) {
            max = grph->nodes[i].p;
        }
    }
    printf("%d %d\n", max, min);
}
