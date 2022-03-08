#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>

#define _PHASE_COUNT 6
#define INIT_SIZE 10

enum place { NUZKY, VRTACKA, OHYBACKA, SVARECKA, LAKOVNA, SROUBOVAK, FREZA, _PLACE_COUNT };
enum product { A, B, C, _PRODUCT_COUNT };

const char *place_str[_PLACE_COUNT] = { [NUZKY] = "nuzky", [VRTACKA] = "vrtacka", [OHYBACKA] = "ohybacka", [SVARECKA] = "svarecka", [LAKOVNA] = "lakovna", [SROUBOVAK] = "sroubovak", [FREZA] = "freza" };
const int operation_time[_PLACE_COUNT] = { [NUZKY] = 100, [VRTACKA] = 200, [OHYBACKA] = 150, [SVARECKA] = 300, [LAKOVNA] = 400, [SROUBOVAK] = 250, [FREZA] = 500};
const char *product_str[_PRODUCT_COUNT] = { [A] = "A", [B] = "B", [C] = "C" };
int parts[_PRODUCT_COUNT][_PHASE_COUNT] = { [A] = {NUZKY, VRTACKA, OHYBACKA, SVARECKA, VRTACKA, LAKOVNA}, [B] = {VRTACKA, NUZKY, FREZA, VRTACKA, LAKOVNA, SROUBOVAK}, [C] = {FREZA, VRTACKA, SROUBOVAK, VRTACKA, FREZA, LAKOVNA}};

// array for counting current phase of making products
int phase[_PRODUCT_COUNT][_PHASE_COUNT] = { [A] = {0}, [B] = {0}, [C] = {0}};
pthread_mutex_t mutex_phase;
int phase_taken[_PRODUCT_COUNT][_PHASE_COUNT] = { [A] = {0}, [B] = {0}, [C] = {0}};
pthread_mutex_t mutex_phase_taken;

// structure for passing "worker name" and "workplace id" to the thread
typedef struct worker {
    char *name;
    int workplace;
    int thread_id;
} worker_t;

// structure for passing a termination "message" to the thread (using boolean variable)
typedef struct thread_name {
    char *name;
    int workplace_id;
    bool has_wrkplc;
    bool finish_work;
    int thread_idx;
    sem_t fin;
}thrd_name_t;

// dynamic array of worker's names (using prev. structure)
thrd_name_t *names;
int names_capacity;
int names_counter;
pthread_mutex_t mutex_names;

// semaphore for signaling the thread to start his work
sem_t sem_make_product[_PLACE_COUNT];
pthread_mutex_t mutex_print;

// semaphore for signaling threads about new free workplace
sem_t sem_new_workplace[_PLACE_COUNT];

// array for free workplaces
int ready_places[_PLACE_COUNT];
pthread_mutex_t mutex_place[_PLACE_COUNT];

// array for signal to free request place
int remove_places[_PLACE_COUNT];
pthread_mutex_t mutex_remove[_PLACE_COUNT];

// boolea variable for EOF
bool stop_work;
pthread_mutex_t mutex_stop_work;

// array for checking presence of worker at request place
int worker_at_place[_PLACE_COUNT];

sem_t *fin;

//==================== FUNCTIONS ====================================
/* function that duplicates a string */
extern char *strdup(const char *);
/* function that find string in array */
int find_string_in_array(const char **, int, char *);
/* initialization all mutexes */
void mutexes_init();
/* destroy all mutexes */
void mutexes_destroy();
/* initialization all semaphores */
void semaphores_init();
/* initialization some arrays */
void arrays_init();
/* function that add name of new worker to array */
void add_worker_name(char *, int, int);
/* function that is passed to threads */
void *start_working(void *);
/* function that simulates workflow */
void product_phase_print(char *, int, int, int);
/* if there IS still WORK before this workplace 
 * and there are ALL workers before this workplace,
 * worker at this workplace stays at work - otherwise he goes home
*/
bool check_finish(int);
/* function that add free workplaces */
void change_ready_places(int);
/* resets the values at the specified position in the 'names' array */
void refresh_names_array(int);


int workers_id_count;
sem_t final;
int first_thread;
//=================== MAIN ==========================================
int main(int argc, char **argv) {
    stop_work = false;

    mutexes_init();
    semaphores_init();
    arrays_init();
    fin = (sem_t*)malloc(sizeof(sem_t) * INIT_SIZE);
    pthread_t *workers_id = (pthread_t *)malloc(sizeof(pthread_t) * INIT_SIZE);
    int workers_id_capacity = INIT_SIZE;
    workers_id_count = 0;

    first_thread = 0;

     
    while (1) {
        char *line, *cmd, *arg1, *arg2, *arg3, *saveptr;
        int s = scanf(" %m[^\n]", &line);
        if (s == EOF) { break; }  
        if (s == 0) { continue; }
            
        cmd  = strtok_r(line, " ", &saveptr);
        arg1 = strtok_r(NULL, " ", &saveptr);
        arg2 = strtok_r(NULL, " ", &saveptr);
        arg3 = strtok_r(NULL, " ", &saveptr);

        //================ START <NAME> <WORKPLACE> =========================
        if (strcmp(cmd, "start") == 0 && arg1 && arg2 && !arg3) {
            char *workplace_name = strdup(arg2);
            int idx = find_string_in_array(place_str, _PLACE_COUNT, workplace_name);

            if (idx >= 0) {
                worker_t *worker = malloc(sizeof(worker_t));
                worker->name = strdup(arg1);
                worker->workplace = idx;

                add_worker_name(worker->name, idx, workers_id_count);
                
                if (workers_id_count == (workers_id_capacity - 1)) {
                    workers_id_capacity += INIT_SIZE;
                    workers_id =(pthread_t *)realloc(workers_id, sizeof(pthread_t) * workers_id_capacity);
                    fin = (sem_t*)realloc(fin, sizeof(sem_t) * workers_id_capacity);
                }
                worker->thread_id = workers_id_count;
                sem_init(&fin[workers_id_count], 0, 0);
                pthread_create(&workers_id[workers_id_count], NULL, start_working, (void*)(worker));
                workers_id_count++;   
            }
            free(workplace_name); 

        //================ MAKE <PRODUCT> ====================================  
        } else if (strcmp(cmd, "make") == 0 && arg1 && !arg2) {

            int product = find_string_in_array(product_str, _PRODUCT_COUNT, arg1);

            if (product >= 0) {
                pthread_mutex_lock(&mutex_phase);
                phase[product][0] += 1;
                pthread_mutex_unlock(&mutex_phase); 
                sem_post(&sem_make_product[parts[product][0]]);            
            }

        //================ END <NAME> ========================================   
        } else if (strcmp(cmd, "end") == 0 && arg1 && !arg2) {

            char *name = strdup(arg1);

            pthread_mutex_lock(&mutex_names);
            for (int i = 0; i < names_capacity; i++) {
                if (names[i].name != NULL) {
                    if (strcmp(names[i].name, name) == 0) {
                        names[i].finish_work = true;
                        sem_post(&sem_new_workplace[names[i].workplace_id]);
                    }
                }  
            }
            pthread_mutex_unlock(&mutex_names);
            
            free(name);

        //================ ADD <WORKPLACE> ===================================
        } else if (strcmp(cmd, "add") == 0 && arg1 && !arg2) {
            
            int idx = find_string_in_array(place_str, _PLACE_COUNT, arg1);
            change_ready_places(idx);
            sem_post(&sem_new_workplace[idx]);

        //================ REMOVE <WORKPLACE> ================================ 
        } else if (strcmp(cmd, "remove") == 0 && arg1 && !arg2) {

            int idx = find_string_in_array(place_str, _PLACE_COUNT, arg1);
            if (idx >= 0) {
                pthread_mutex_lock(&mutex_place[idx]);
                if (ready_places[idx] > 0) {
                    ready_places[idx]--;
                }
                else {
                    pthread_mutex_lock(&mutex_remove[idx]);
                    remove_places[idx]++;
                    pthread_mutex_unlock(&mutex_remove[idx]);

                    sem_post(&sem_make_product[idx]);
                }
                pthread_mutex_unlock(&mutex_place[idx]);
            }
                 
        } else {
            fprintf(stderr, "Invalid command: %s\n", line);
        }
        free(line);
    }
    //==================== FINISH ===================================
    stop_work = true;

    /* send sem_post to all active workers:
     * 1) if worker has workplace(he's waiting for work) - send him signal about new work
     * 2) if worker doesnt have workplace(he's waiting for it) - send him signal about new workplace
    */
    pthread_mutex_lock(&mutex_names);
    for (int i = 0; i < names_capacity; i++) {
        if (names[i].name != NULL) {
            if(names[i].has_wrkplc) {
                sem_post(&sem_make_product[names[i].workplace_id]);
            }
            else {
                sem_post(&sem_new_workplace[names[i].workplace_id]);
                sem_post(&sem_make_product[names[i].workplace_id]);
            }
        }
    }
    pthread_mutex_unlock(&mutex_names);


    /* join all threads */
    for (int i = 0; i < workers_id_count; i++) {
        sem_post(&fin[i]);
        pthread_join(workers_id[i], NULL);
    }
    free(workers_id);

    mutexes_destroy();
    return 0;
}

int find_string_in_array(const char **array, int length, char *what) {
    for (int i = 0; i < length; i++) {
        if (strcmp(array[i], what) == 0) {
            return i;
        }  
    }  
    return -1;
}

void mutexes_init() {
    for (int i = 0; i < _PLACE_COUNT; i++) {
        pthread_mutex_init(&mutex_place[i], NULL);
        pthread_mutex_init(&mutex_remove[i], NULL);
    }

    pthread_mutex_init(&mutex_names, NULL);
    pthread_mutex_init(&mutex_phase, NULL);
    pthread_mutex_init(&mutex_print, NULL);
    pthread_mutex_init(&mutex_stop_work, NULL);
}

void mutexes_destroy() {
    for (int i = 0; i < _PLACE_COUNT; i++) {
        pthread_mutex_destroy(&mutex_place[i]);
        pthread_mutex_destroy(&mutex_remove[i]);
    }
    pthread_mutex_destroy(&mutex_names);
    pthread_mutex_destroy(&mutex_phase);
    pthread_mutex_destroy(&mutex_print);
    pthread_mutex_destroy(&mutex_stop_work);
}

void semaphores_init() {
    for (int i = 0; i < _PLACE_COUNT; i++) {
        sem_init(&sem_make_product[i], 0, 0);
        sem_init(&sem_new_workplace[i], 0, 0);
    }
}

void arrays_init() {
    for (int i = 0; i < _PLACE_COUNT; i++) {
        ready_places[i] = 0;
        remove_places[i] = 0;
        worker_at_place[i] = 0;
    }
     
    names_capacity = INIT_SIZE;
    names_counter = 0;

    names = malloc(sizeof(thrd_name_t) * names_capacity);
    for (int i = 0; i < names_capacity; i++) {
        names[i].name = NULL;
        names[i].workplace_id = -1;
        names[i].has_wrkplc = false;
        names[i].finish_work = false;
        names[i].thread_idx = -1;
    }
}

void add_worker_name(char *worker_name, int idx, int workers_id_count) {
    pthread_mutex_lock(&mutex_names);
    if (names_counter == (names_capacity - 1)) {
        names_capacity += INIT_SIZE;
        names = realloc(names, sizeof(thrd_name_t) * names_capacity);
    }
    
    for (int i = 0; i < names_capacity; i++) {
        if (names[i].name == NULL) {
            names[i].name = worker_name;
            names[i].workplace_id = idx;
            names[i].thread_idx = workers_id_count;
            break;
        }
    }
    names_counter++;
    pthread_mutex_unlock(&mutex_names);
}

void *start_working(void *worker) {
    /* get worker name and index of workplace */
    //sleep(1);
    worker_t *w = (worker_t*)worker;
    char *name = w->name;
    int place_idx = w->workplace;
    int thread_idx = w->thread_id;

    bool has_workplace = false;
    bool finish = false;

    /* start working */
    while(1) {
        //=================== ADD =========================================================
        /* if worker doesnt have his workplace, he waits signal */
        if (!has_workplace) {
           sem_wait(&sem_new_workplace[place_idx]); 
        }

        /* if worker get signal about adding new workplace, he try to get this workplace */
        pthread_mutex_lock(&mutex_place[place_idx]);      
        if (!has_workplace && ready_places[place_idx]) {
            ready_places[place_idx]--;
            worker_at_place[place_idx]++;
            has_workplace = true;

            // worker finds his name and "says" that now he has workplace
            pthread_mutex_lock(&mutex_names);
            for (int i = 0; i < names_capacity; i++) {
                if ((names[i].name != NULL) && (strcmp(names[i].name, name) == 0)) {
                    names[i].has_wrkplc = true;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex_names);
        }
        pthread_mutex_unlock(&mutex_place[place_idx]);

        // =============== MAKE =============================================================
        /* if worker has workplace, he waits for work */
        if (has_workplace) {
            sem_wait(&sem_make_product[place_idx]);
            bool change = false;

            for (int i = 5; i >= 0; i--) {
                for (int j = 0; j < _PRODUCT_COUNT; j++) { 
                    
                    if (phase[j][i] && (parts[j][i] == place_idx) ) {
                        pthread_mutex_lock(&mutex_phase_taken);
                        if (phase[j][i] > phase_taken[j][i]) {
                            phase_taken[j][i]++;
                            change = true;
                        }
                        pthread_mutex_unlock(&mutex_phase_taken);
                    }
                    if (change) {
                        change = false;
                        product_phase_print(name, place_idx, i, j);
                    }

                }    
            } 
        }

        //================= END ========================================================
        /* worker checks if HE needs to leave */
        pthread_mutex_lock(&mutex_names);
        for (int i = 0; i < names_capacity; i++) {
            if ((names[i].name != NULL) && (strcmp(names[i].name, name) == 0) && names[i].finish_work) {
                // clean his structure and free his workplace
                refresh_names_array(i);
                change_ready_places(place_idx);
                
                finish = true;
                has_workplace = false;
                worker_at_place[place_idx]--;
                break;
            }
        }
        pthread_mutex_unlock(&mutex_names);

        if (finish) { 
            free(w->name);
            free(w);
            if (thread_idx == first_thread) {
                first_thread++;
            }
            sem_post(&final);
            pthread_exit(NULL);
            break; 
        }

        //=============== REMOVE =============================================================
        /* worker checks if there is request to remove this workplace */
        pthread_mutex_lock(&mutex_remove[place_idx]);
        if (remove_places[place_idx] && has_workplace) {
            has_workplace = false;
            remove_places[place_idx]--;
            worker_at_place[place_idx]--;

            pthread_mutex_lock(&mutex_names);
            for (int i = 0; i < names_capacity; i++) {
                if ((names[i].name != NULL) && (strcmp(names[i].name, name) == 0)) {
                    names[i].has_wrkplc = false;
                }
            }
            pthread_mutex_unlock(&mutex_names);
        }
        pthread_mutex_unlock(&mutex_remove[place_idx]);

        // ============= EOF =================================================================
        /* worker checks: 
         *      1) if there is EOF; 
         *      2) if he needs to wait for work or he can leave. 
         */
        
        if (stop_work) {
            pthread_mutex_lock(&mutex_stop_work);
            finish = check_finish(place_idx); 
            pthread_mutex_unlock(&mutex_stop_work);    
        }
        
        if (finish) { 
            worker_at_place[place_idx]--;
            break; 
        }

    }
    
    free(w);
    if (thread_idx == first_thread) {
        for (int i = 1; i < workers_id_count; i++) {
            sem_wait(&final);
        }     
    }
    else {
        sem_post(&final);
    }
    sem_wait(&fin[thread_idx]);
    return (void*)0;
}

void product_phase_print(char *worker, int place_idx, int work_phase, int product) {
    
    pthread_mutex_lock(&mutex_print);
    printf("%s %s %d %s\n", worker, place_str[place_idx], work_phase+1, product_str[product]);
    pthread_mutex_unlock(&mutex_print);

    usleep(operation_time[place_idx] * 1000);

    if ((work_phase + 1) == _PHASE_COUNT) {
        pthread_mutex_lock(&mutex_phase);
        phase[product][work_phase] -= 1;
        pthread_mutex_unlock(&mutex_phase);

        pthread_mutex_lock(&mutex_phase_taken);
        phase_taken[product][work_phase] -= 1;
        pthread_mutex_unlock(&mutex_phase_taken);

        pthread_mutex_lock(&mutex_print);
        printf("done %s\n", product_str[product]);
        pthread_mutex_unlock(&mutex_print);
    }
    else {
        pthread_mutex_lock(&mutex_phase);
        phase[product][work_phase] -= 1; 
        phase[product][work_phase+1] += 1;
        pthread_mutex_unlock(&mutex_phase);

        pthread_mutex_lock(&mutex_phase_taken);
        phase_taken[product][work_phase] -= 1;
        pthread_mutex_unlock(&mutex_phase_taken);

        sem_post(&sem_make_product[parts[product][work_phase+1]]);
    }

}

bool check_finish(int place_idx) {

    bool finish = true;
    
    /* --------------------------------------------------- */
    pthread_mutex_lock(&mutex_phase);
    int product = 0;
    NEXT: while (product < _PRODUCT_COUNT) {

        if (product >= _PRODUCT_COUNT) {break;}

        int to = -1;
        for (int idx = 0; idx < _PHASE_COUNT; idx++) {
            if (parts[product][idx] == place_idx) {
                to = idx;
                break;
            }
        }
        if (to == -1) { // worker doesnt has phase for this product
            product++;
            goto NEXT;
        } 
        int from = 0;
        pthread_mutex_lock(&mutex_phase_taken);
        while(1) { 
            if (phase[product][from] == 0) {
               from++; 
            }
            else if (phase[product][from] == phase_taken[product][from]) {
                from++;
                break;
            }
            else {
                break;
            }
            if (from == _PHASE_COUNT) { break; } 
        }
        pthread_mutex_unlock(&mutex_phase_taken);
        if (from == _PHASE_COUNT) { 
            product++;
            goto NEXT;
        }
        if (from > to) { // check if this worker has another phase for this product
            for (int idx = from; idx < _PHASE_COUNT; idx++){
                if (parts[product][idx] == place_idx) {
                    to = idx;
                    break;
                }
            }        
        }
        if (from > to) { 
            product++;
            goto NEXT;
        }

        bool cant_finish_prod = false;
        for (int j = from; j <= to; j++) {
            pthread_mutex_lock(&mutex_place[parts[product][j]]);
            if (worker_at_place[parts[product][j]] <= 0) {
                if (ready_places[parts[product][j]] == 0) {
                    cant_finish_prod = true;
                }
                else {
                    bool worker = true;
                    pthread_mutex_lock(&mutex_names);
                    for (int i = 0; i < names_capacity; i++) {
                        worker = false;
                        if (names[i].name != NULL) {
                            if(names[i].workplace_id == parts[product][j]) {
                                worker = true;
                                break;   
                            }
                        }
                    }
                    pthread_mutex_unlock(&mutex_names);
                    if (!worker) {
                        cant_finish_prod = true;
                    }
                }
            }    
            pthread_mutex_unlock(&mutex_place[parts[product][j]]);
        }

        if (!cant_finish_prod) {
            finish = false;
            break;
        }
        product++;

    } 
    
    pthread_mutex_unlock(&mutex_phase);
    return finish;
}

void change_ready_places(int idx) {
    pthread_mutex_lock(&mutex_place[idx]);
    ready_places[idx]++;
    pthread_mutex_unlock(&mutex_place[idx]);
}

void refresh_names_array(int idx) {
    names[idx].name = NULL;
    names[idx].workplace_id = -1;
    names[idx].has_wrkplc = false;
    names[idx].finish_work = false;
    names_counter--;
}
