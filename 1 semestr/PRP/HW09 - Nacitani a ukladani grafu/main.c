#include "stdio.h"
#include "stdlib.h"

#include "graph.h"

int main(int argc, char *argv[]) {
    graph_t *graph;
    graph = allocate_graph();
    free_graph(graph);
    return 0;
}