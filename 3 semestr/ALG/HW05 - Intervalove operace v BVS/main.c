#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t {
    int key;
    struct node_t *left_child;
    struct node_t *right_child;
}node_t;

typedef struct graph {
    int num_of_nodes;
    int depth;
    node_t *root;
}graph_t;

void create_graph(graph_t *);
node_t* create_node(graph_t *, int);
node_t* delete_node(graph_t *, node_t*, int);
void grph_push_node(graph_t*, int, int);
void push_node(graph_t*, node_t*, int, int);
void grph_pull_node(graph_t*, node_t*, int, int);
node_t* find_minimun(node_t *);
void search_depth(graph_t *, node_t *, int);


int main(int argc, char *argv[])  {

    int a = 0, b = 0;
    int num_of_operations = 0;
    char operation;

    graph_t graph;
    create_graph(&graph);
    
    scanf("%d", &num_of_operations);

    for (int i = 0; i < num_of_operations; i++) {
        scanf(" %c %d %d", &operation, &a, &b);
        switch (operation) {
            case 'i':
                grph_push_node(&graph, a, b);
                break;

            case 'd':
                grph_pull_node(&graph, graph.root, a, b);
                break;
        }
    }
    search_depth(&graph, graph.root, 0);
    printf("%d %d\n", graph.num_of_nodes, graph.depth);
    return 0;
}

void create_graph(graph_t *grph) {

  grph->root = NULL;
  grph->num_of_nodes = 0;
  grph->depth = 0;

}

node_t* create_node(graph_t *grph, int num) {

    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->key = num;
    node->left_child = NULL;
    node->right_child = NULL;

    grph->num_of_nodes ++;

    return node;

}

void grph_push_node(graph_t* graph, int a, int b) {

    if (graph->root) {
        push_node(graph, graph->root, a, b);
    }
    else {
        graph->root = create_node(graph, a);
        push_node(graph, graph->root, a+1, b);
    }
    
}

void push_node(graph_t *grph, node_t* node, int a, int b) {

    node_t *parent = node;
    bool border = false;

    if(b <= node->key || a >= node->key) {
        a = (a == node->key) ? a + 1 : a;
        b = (b == node->key) ? b - 1 : b;
        border = true;
    }

    // if node doesnt have children
    if(!(node->left_child) && !(node->right_child)) {
        for (int i = a; i <= b; i++) {
            if (i == node->key) {
                parent = node;
            }
            else {
                if (i < parent->key) {
                    parent->left_child = create_node(grph, i);
                    parent = parent->left_child;
                }
                else {
                    parent->right_child = create_node(grph, i);
                    parent = parent->right_child;
                } 
            }                  
        }                       
    }
    // if root has 2 children
    else if (node->left_child && node->right_child) {
        if(border) {
            if(b < node->key) {
                push_node(grph, node->left_child, a, b);
            }
            else {
                push_node(grph, node->right_child, a, b);
            }
        }
        else {
            push_node(grph, node->left_child, a, (node->key - 1));
            push_node(grph, node->right_child, (node->key + 1), b);
        }         
    }
    // if node has left children
    else if (node->left_child){
        if (border) {
            if (b < node->key) {
                push_node(grph, node->left_child, a, b);
            }
            else {
                for (int i = a; i <= b; i++) {
                    parent->right_child = create_node(grph, i);
                    parent = parent->right_child;
                }
            }
        }
        else {
            push_node(grph, node->left_child, a, (node->key-1));
            for (int i = (node->key + 1); i <= b; i++) {
                parent->right_child = create_node(grph, i);
                parent = parent->right_child;
            }
        }
    }
    // if node has right children
    else {
        if(border) {
            if (a > node->key) {
                push_node(grph, node->right_child, a, b);
            }
            else {
                for (int i = a; i <= b; i++) {
                    if (i < parent->key) {
                        parent->left_child = create_node(grph, i);
                        parent = parent->left_child;
                    }
                    else {
                        parent->right_child = create_node(grph, i);
                        parent = parent->right_child;
                    }
                }
            }
        }
        else {
            for (int i = a; i < node->key; i++) {
                if (i < parent->key) {
                    parent->left_child = create_node(grph, i);
                    parent = parent->left_child;
                }
                else {
                    parent->right_child = create_node(grph, i);
                    parent = parent->right_child;
                }
            }
            push_node(grph, node->right_child, (node->key + 1), b);
        }
    }
}

void grph_pull_node(graph_t* graph, node_t *node, int a, int b) {

    if (node->left_child && a < node->key){
        grph_pull_node(graph, node->left_child, a, b);      
    } 
    if (node->right_child && b > node->key){
        grph_pull_node(graph, node->right_child, a, b);
    }
    
    while (node->right_child && (a <= node->right_child->key) && (node->right_child->key <= b)) {
        node = delete_node(graph, node, node->right_child->key);    
    }

    while (node->left_child && (a <= node->left_child->key) && (node->left_child->key <= b)) {
        node = delete_node(graph, node, node->left_child->key);
    }
             
    if (node->key == graph->root->key && a <= node->key && node->key <= b) {
        graph->root = delete_node(graph, node, node->key);
    }
    
}

node_t* find_minimun(node_t *node) {
    if (node) {
        if (node->left_child) {
            return find_minimun(node->left_child);
        } else {
            return node;
        }
    } else {
        return NULL;
    }
}
 
node_t* delete_node(graph_t* grph, node_t* node, int num) {
    if (node) {
        if (num == node->key) {   
            if (!(node->left_child) && !(node->right_child)) {
                free(node);
                grph->num_of_nodes--;
                return NULL;
            } else {
                if (node->left_child && node->right_child) {
                    node_t *min = find_minimun(node->right_child);
                    node->key = min->key;
                    node->right_child = delete_node(grph, node->right_child, node->key);
                } else {
                    node_t *new_child = node->left_child ? node->left_child : node->right_child;
                    free(node);
                    grph->num_of_nodes--;
                    return new_child;
                }
            }
        } else if (num < node->key) {
            node->left_child = delete_node(grph, node->left_child, num);
        } else {
            node->right_child = delete_node(grph, node->right_child, num);
        }
        return node;
    } else {
        return NULL;
    }
}

void search_depth(graph_t *grph, node_t *node, int d) {

    if(node->left_child) {
        search_depth(grph, node->left_child, d+1);
    }
    if (node->right_child) {
        search_depth(grph, node->right_child, d+1);
    }
    if (d > grph->depth) {
        grph->depth = d;
    }
}
