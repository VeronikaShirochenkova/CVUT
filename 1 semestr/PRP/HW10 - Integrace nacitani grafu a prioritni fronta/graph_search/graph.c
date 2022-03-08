#define _POSIX_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <netinet/in.h>


#include "graph.h"
#include "my_malloc.h"

//#include <stdint .h>

#define IS_BIG_ENDIAN (* (uint16_t *) "\ 0 \ xff" <0x100)

#define INIT_SIZE 10

#define FROM 0
#define TO 1
#define COST 2

#define OPTIONS 3

#define TEN 10
#define SPACE ' '
#define NEXT_LINE '\n'

graph_t* allocate_graph(void) 
{
   graph_t *graph = myMalloc(sizeof(graph_t));
   graph->edges = NULL;
   graph->num_edges = 0;
   graph->capacity = 0;
   return graph;
}

void free_graph(graph_t **graph) 
{
   assert(graph != NULL && *graph != NULL);
   if ((*graph)->capacity > 0) {
      free((*graph)->edges);
   }
   free(*graph);
   *graph = NULL;
}

graph_t *enlarge_graph(graph_t *graph) {

   int n = graph->capacity == 0 ? INIT_SIZE : graph->capacity * 2;
   graph->capacity = n;
   
   graph->edges = realloc(graph->edges, graph->capacity * sizeof(edge_t));
   if (graph->edges == NULL) {
      return NULL;
   }
   return graph;
}

void load_txt(const char *fname, graph_t *graph) {

   FILE *f;
   char ch;
   int counter = 0;

   if ((f = fopen(fname, "r")) != NULL) {

      int num = 0;
      while((ch = getc_unlocked(f)) != EOF) {  

         if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
         }

         edge_t *edge = graph->edges;

         if(ch >= '0' && ch <= '9') {
            num = (num == 0) ? (ch - '0') : (num * 10 + (ch - '0'));            
         }
         else {
            if (counter < OPTIONS) {
               if (counter == FROM) {
                  edge[graph->num_edges].from = num;
               }
               if (counter == TO) {
                  edge[graph->num_edges].to = num;
               }
               if (counter == COST) {
                  edge[graph->num_edges].cost = num;
               }
               counter++;
               num = 0;
            }
            if (counter > 2) {
               graph->num_edges++;
               counter = 0;
               num = 0;
            }
         }
      }
   }

   if (f) {
      fclose(f);
   }
}

void load_bin(const char *fname, graph_t *graph)
{
   bool exit = false;
   FILE *f = fopen(fname, "rb");

   while (f && !exit) {

      if (graph->num_edges == graph->capacity) {
         enlarge_graph(graph);
      }

      while (graph->num_edges < graph->capacity) {
         int r = fread(&graph->edges[graph->num_edges], sizeof(edge_t), 1, f);
         if (r == 1) {
            graph->num_edges += 1;
         }
         else {
            exit = true;
            break;
         }
      }
   }
   if (f) {
      fclose(f);
   }
}

void save_txt(const graph_t *const graph, const char *fname) {

   FILE *f = fopen(fname, "w+");
   if (f == NULL) {
      exit(-1);
   }
   int arr[OPTIONS];

   for (int idx = 0; idx < graph->num_edges; idx++) {
      arr[FROM] = graph->edges[idx].from;
      arr[TO] = graph->edges[idx].to;
      arr[COST] = graph->edges[idx].cost;
    
      for (int j = 0; j < OPTIONS; j++) {
         if(arr[j] < TEN) {
            putc_unlocked((arr[j] + '0'), f);
         }
         else {
            int n = arr[j];
            int count = 0;

            while (n / TEN) {
               n = n / TEN;
               count++;
            }
          
            int pow = 10;
            while(count > 1) {
               pow *= 10;
               count--;
            }

            while (pow) {
               putc_unlocked((arr[j] / pow + '0'), f);
               arr[j] = arr[j] % pow;
               pow = pow / 10;
            }            
         }
         if (j < 2) {
            putc_unlocked(SPACE, f);
         }
         else {
            putc_unlocked(NEXT_LINE, f);
         }
      }              
   }     

   fclose(f);
}

void save_bin(const graph_t *const graph, const char *fname) {
   
   FILE *f = fopen(fname, "wb");
   if (f == NULL) {
      exit(-1);
   }
   for (int i = 0; i < graph->num_edges; i++) {
      fwrite(&graph->edges[i], sizeof(edge_t), 1, f);
   }
   fclose(f);
}
