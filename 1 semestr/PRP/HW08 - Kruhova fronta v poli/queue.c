#include "queue.h"

queue_t *create_queue(int capacity) {
    queue_t *queue = malloc(sizeof(queue_t));
    queue->items = malloc(capacity * sizeof(void*));
    queue->maxsize = capacity;
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    return queue;
}
  
void delete_queue(queue_t *queue) {
    free(queue->items);
    free(queue);
}
  
bool push_to_queue(queue_t *queue, void *data) {
    if ( queue->size == queue->maxsize) {

        queue->maxsize = queue->maxsize * 2;
        queue->items = realloc(queue->items, queue->maxsize * sizeof(void*));

        if(queue->head != 0) {
            while(queue->head != 0) {
                void *el = queue->items[0];

                for (int i = 0; i < queue->maxsize-1; i++) {
                    queue->items[i] = queue->items[i+1];
                }

                queue->items[queue->size - 1] = el;
                queue->head --;

                if(queue->head == 0) {
                    queue->tail = queue->size;
                }
            }          
        }

        queue->items[(queue->tail++) % queue->maxsize] = data;
        queue->size++;         
    }
    else {
        queue->items[(queue->tail++)  % queue->maxsize] = data;
        queue->size++;         
    }
    return true;
}
  
void* pop_from_queue(queue_t *queue) {
    if (queue->size > 0 ) { 
        if(queue->maxsize > 4 && queue->size < (queue->maxsize - (queue->maxsize/3))) {
            if (queue->head == 0) {
                queue->maxsize = queue->maxsize - (queue->maxsize / 3);
                queue->items = realloc(queue->items, queue->maxsize * sizeof(void*));
                queue->size--;
                return queue->items[(queue->head++) % queue->maxsize]; // ++ ?
            }
            // else {
            //     if (queue->head <= queue->tail) {
            //         while (queue->head > 0) {

            //             for (int i = queue->head; i <= queue->tail; i++) {
            //                 queue->items[i-1] = queue->items[i];
            //             }

            //             queue->head--;
            //             queue->tail--;
            //         }
            //     }
            //     else if (queue->tail < queue->head) {
            //         while(queue->head != 0) {
            //             void *el = 0; 

            //             if (queue->tail < queue->head) {
            //                 el = queue->items[0];
            //             }
                        
            //             int to = (queue->tail > queue->head) ? (queue->tail + 1) : queue->maxsize;
            //             for (int i = queue->head; i <= to; i++) {
            //                 queue->items[i-1] = queue->items[i];
            //                 if (i == to) {
            //                     if (queue->tail == to) {
            //                         queue->tail--;
            //                     }
            //                     else {
            //                         queue->items[i] = el;
            //                     }                               
            //                 }                          
            //             }

            //             queue->head --; 

            //             if (queue->tail != -1 && queue->tail < queue->head) {
            //                 queue->tail--;
            //             }

            //             if(queue->tail == -1) {
            //                 queue->tail = queue->maxsize -1;
            //             }                   
            //         }                    
            //     }
            //     queue->maxsize = queue->maxsize - (queue->maxsize / 3);
            //     queue->items = realloc(queue->items, queue->maxsize * sizeof(void*));
            //     queue->size--;
            //     return queue->items[(queue->head++) % queue->maxsize];
            // }
        }
        queue->size--;
        return queue->items[(queue->head++) % queue->maxsize];
    }
    else {
        return NULL;    
    }
}
  
void* get_from_queue(queue_t *queue, int idx) {
    if (idx < 0 || queue->size == 0 || idx >= queue->maxsize || (queue->head + idx) >= queue->tail ) {
        return NULL;
    } else {
        return queue -> items [(queue->head + idx) % queue->maxsize ];
    }
}
  
int get_queue_size(queue_t *queue) {
  return queue->size;
}
