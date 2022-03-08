#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>

#define MAX_NUM_THREADS ((int)sysconf(_SC_NPROCESSORS_ONLN))
 
typedef struct element_t{
  struct element_t *previous;
  struct element_t *next;
  int num;
  char *txt;
} element_t;
 
typedef struct list {
  element_t *head;
  element_t *tail;
  int counter;
} list_t;

typedef struct index {
    int id;
} index_t;

list_t* list_create();
element_t* element_create(int, char*);
void element_delete(element_t*);
void element_in(list_t*, element_t*);
void element_out(list_t*, element_t**);
void *read_input();
void *print_input(void *);

list_t* lst;
pthread_mutex_t mutex, mutex2;
sem_t semaphore;

bool stop = false;


int main(int argc, char *argv[]) {
  
  pthread_t producent;
  pthread_t *consument;
  
  void *status;

  /* num of threads */
  int num_of_threads = 1;
  if ( argv[1] ) {
    if(atoi(argv[1]) > MAX_NUM_THREADS) { 
      exit(1); 
    }
    num_of_threads = atoi(argv[1]);
  }

  /* create list */
  lst = list_create();

  /* allocate memory */
  consument = (pthread_t *)malloc(sizeof(pthread_t) * num_of_threads);

  /* create indexes for threads */
  index_t args[num_of_threads];
  for (int i = 0; i < num_of_threads; i++) {
    args[i].id = i + 1;
  }
  
  /* mutex initialization */
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex2, NULL);

  // semaphore initialization
  sem_init(&semaphore, 0, 0);

  /* create producent thread */
  pthread_create(&producent, NULL, &read_input, NULL);
  /* create consument threads */
  for (int i = 0; i < num_of_threads; i++) {
      pthread_create(&consument[i], NULL, &print_input, (void*) &args[i]);
  }

  /* termination producent thread */
  pthread_join(producent, &status);
  for (int i = 0; i < num_of_threads; i++) {
    sem_post(&semaphore);
  }
  /* termination consument threads */
  for (int i = 0; i < num_of_threads; i++) {
    pthread_join(consument[i], NULL);
  }

  /* termination mutex */
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);

  /* free memory */
  free(consument);
  free(lst);

  if (stop) {
    return 1;
  }
  return 0;
}

list_t* list_create() {

  list_t* lst = (list_t*)malloc(sizeof(list_t));
  lst->head = NULL;
  lst->tail = NULL;
  lst->counter = 0;
  return lst;

}

element_t* element_create(int num, char* text) {

  element_t* elm = (element_t*)malloc(sizeof(element_t));
  elm->next = NULL;
  elm->previous = NULL;
  elm->num = num;
  elm->txt = text;
  return elm;

}

void element_delete(element_t* elm) {

  free(elm->txt);
  elm->txt = NULL;
  free(elm);
  elm = NULL;

}

void element_in(list_t* lst, element_t* elm) {

  pthread_mutex_lock(&mutex);  
  if (lst->head == NULL) {
      lst->head = elm;
  }
  else {
      elm->previous = lst->tail;
      lst->tail->next = elm;
  }
      
  lst->counter++;
  lst->tail = elm;
  pthread_mutex_unlock(&mutex);

}

void element_out(list_t* lst, element_t** elm) {
  
  pthread_mutex_lock(&mutex);
  *elm = lst->head;
  
  if (lst->counter > 1) {
    lst->head = lst->head->next;
  }
  if (lst->counter == 1) {
    lst->head = NULL;
    lst->tail = NULL;
  }

  lst->counter--;
  pthread_mutex_unlock(&mutex);

}

void *read_input() {
  
  int ret, x;
  char *text;
  element_t* elm;

  while ((ret = scanf("%d %ms", &x, &text)) == 2) {
    if(text == NULL || x < 0) {
      free(text);
      stop = true;
      break;
    }
    elm = element_create(x, text);
    element_in(lst, elm);
    sem_post(&semaphore);


  }

  if (ret != EOF) {
    stop = true;
  }
  
  return NULL;
}

void *print_input(void *args) {

  element_t* elm = NULL;

  while (1) {
    sem_wait(&semaphore);
    element_out(lst, &elm);
    if (!elm) { 
      break; 
    }
    index_t *arg = (index_t *) args;

    pthread_mutex_lock(&mutex2);
    printf("Thread %d:", arg->id);
    for (int i = 0; i < elm->num; i++) {
      printf(" %s", elm->txt);
    }      
    printf("\n");

    pthread_mutex_unlock(&mutex2);
    element_delete(elm);
  }
  return NULL;
}
