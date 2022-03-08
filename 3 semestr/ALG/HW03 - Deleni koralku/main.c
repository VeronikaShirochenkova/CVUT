#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
typedef struct node_t{
    int num;
    int color;
    struct node_t *node1;
    int ret_from_n1;
    struct node_t *node2;
    int ret_from_n2;
    struct node_t *node3;
    int ret_from_n3;
}node_t;

typedef struct graph {
    int num_nodes;
    int num_red;
    node_t *nodes;
}graph_t;

typedef struct diff {
    int node_1st;
    int node_2nd;
    int final_diff;
} diff_t;

/* allocate memory */
void graph_alloc(graph_t*);
/* create graph */
void graph_create(graph_t*);
/* read graph nodes */
void graph_scanf(graph_t*);
/* compare found difference with global one */
void compare (int, int, int, diff_t*);
/* create "global" variables for calculation */
void diff_create(diff_t*, graph_t*);
/* recursive graph traversal */
int calculation (int, int, graph_t*, diff_t*);

int main(int argc, char *argv[]) {

    graph_t graph;
    scanf("%d %d", &graph.num_nodes, &graph.num_red);
    graph_alloc(&graph);
    graph_create(&graph);
    graph_scanf(&graph);

    diff_t difference;
    diff_create(&difference, &graph);
    
    calculation(1,0, &graph, &difference);
    printf ("%d %d\n", difference.node_1st, difference.node_2nd);

    free(graph.nodes);

    return 0;

}

void graph_alloc(graph_t* grph) {
    grph->nodes = (node_t*)malloc(sizeof(node_t) * (grph->num_nodes + 1));
} 

void graph_create(graph_t* grph) {

    for (int i = 1; i <= grph->num_nodes; i++) {
        grph->nodes[i].num = i;

        if (i <= grph->num_red) {
            grph->nodes[i].color = 1;
        }
        else {
            grph->nodes[i].color = 0;
        }
            
        grph->nodes[i].node1 = NULL;
        grph->nodes[i].node2 = NULL;
        grph->nodes[i].node3 = NULL;

        grph->nodes[i].ret_from_n1 = 0;
        grph->nodes[i].ret_from_n2 = 0;
        grph->nodes[i].ret_from_n3 = 0;

    }   
}

void graph_scanf(graph_t* grph) {

    int a = 0, b = 0;

    for (int i = 0; i < (grph->num_nodes - 1); i++) {
        scanf("%d %d", &a, &b);

        if (!grph->nodes[a].node1) {
            grph->nodes[a].node1 = &grph->nodes[b];
        }
        else if (!grph->nodes[a].node2) {
            grph->nodes[a].node2 = &grph->nodes[b];
        }
        else if (!grph->nodes[a].node3) {
            grph->nodes[a].node3 = &grph->nodes[b];
        }        
        
        if (!grph->nodes[b].node1) {
            grph->nodes[b].node1 = &grph->nodes[a];
        }
        else if (!grph->nodes[b].node2) {
            grph->nodes[b].node2 = &grph->nodes[a];
        }
        else if (!grph->nodes[b].node3) {
            grph->nodes[b].node3 = &grph->nodes[a];
        } 
    }
}

void compare (int start, int from, int diff, diff_t *dff) {

    if (diff == dff->final_diff) {
        if(start < from && start <= dff->node_1st && from <= dff->node_2nd) {
            dff->node_1st = start;
            dff->node_2nd = from;
            dff->final_diff = diff;    
        }
        if (start > from && from <= dff->node_1st && start <= dff->node_2nd) {
            dff->node_1st = from;
            dff->node_2nd = start;
            dff->final_diff = diff;
        }
    }
    if (diff < dff->final_diff) {
        if (start < from) {
            dff->node_1st = start;
            dff->node_2nd = from;
        }
        if (start > from) {
            dff->node_1st = from;
            dff->node_2nd = start;
        }
        dff->final_diff = diff;
    }
}

void diff_create(diff_t* difference, graph_t* grph) {
    difference->node_1st = grph->num_nodes + 1;
    difference->node_2nd = grph->num_nodes + 1;
    difference->final_diff = grph->num_red;
}

int calculation (int start, int from, graph_t *grph, diff_t *dff) { 

    int diff = 0;
    // 1st neighbor
    if (grph->nodes[start].node1 && grph->nodes[start].node1->num != from && grph->nodes[start].ret_from_n1 == 0) {
        grph->nodes[start].ret_from_n1 = calculation(grph->nodes[start].node1->num, grph->nodes[start].num, grph, dff);
    }
    // 2nd neighbor
    if (grph->nodes[start].node2 && grph->nodes[start].node2->num != from && grph->nodes[start].ret_from_n2 == 0) {      
        grph->nodes[start].ret_from_n2 = calculation(grph->nodes[start].node2->num, grph->nodes[start].num, grph, dff);
    }
    // 3rd neighbor
    if (grph->nodes[start].node3 && grph->nodes[start].node3->num != from && grph->nodes[start].ret_from_n3 == 0) {
        grph->nodes[start].ret_from_n3 = calculation(grph->nodes[start].node3->num, grph->nodes[start].num, grph, dff);
    }

    // back to 1st neighbor
    if (grph->nodes[start].node1 && grph->nodes[start].node1->num == from) {

        grph->nodes[start].ret_from_n1 = grph->num_red - (grph->nodes[start].ret_from_n2 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color);
        diff = grph->nodes[start].ret_from_n1 - (grph->nodes[start].ret_from_n2 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color);
        
        if (diff < 0) {
            diff *= -1;
        }
        compare(start, from, diff, dff);

        return grph->nodes[start].ret_from_n2 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color;
        
    }
    
    // back to 2nd neighbor
    if (grph->nodes[start].node2 && grph->nodes[start].node2->num == from) {
        grph->nodes[start].ret_from_n2 = grph->num_red - (grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color);
        diff = grph->nodes[start].ret_from_n2 - (grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color);
        
        if (diff < 0) {
            diff *= -1;
        }     
        compare(start, from, diff, dff);
          
        return grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n3 + grph->nodes[start].color;
        
    }
    
    // back to 3rd neighbor
    if (grph->nodes[start].node3 && grph->nodes[start].node3->num == from) {
        grph->nodes[start].ret_from_n3 = grph->num_red - (grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n2 + grph->nodes[start].color);
        diff = grph->nodes[start].ret_from_n3 - (grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n2 + grph->nodes[start].color);
        
        if (diff < 0) {
            diff *= -1;
        }
        compare(start, from, diff, dff);

        return grph->nodes[start].ret_from_n1 + grph->nodes[start].ret_from_n2 + grph->nodes[start].color;      
    }  
    
    return 0;
}
