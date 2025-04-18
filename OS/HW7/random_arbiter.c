#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
random_arbiter.c:  
    modified version of random_noprotection.c that implements the arbiter (waiter) solution.

gcc -o random_arbiter random_arbiter.c && ./random_arbiter
*/

/* =========================================================================== */

#define PHILOSOPHER_COUNT 10
#define AVAILABLE_MEALS 10000000
pthread_t philosopher[PHILOSOPHER_COUNT];
pthread_mutex_t chopstick[PHILOSOPHER_COUNT];

int MEALS_ON_TABLE = AVAILABLE_MEALS;
pthread_mutex_t MEAL_COUNT_LOCK;

typedef struct {
    int philosopher_state[PHILOSOPHER_COUNT];   // 0 - thinking, 1 - waiting, 2 - eating
    pthread_mutex_t mutex;
    pthread_cond_t cond[PHILOSOPHER_COUNT];
} arbiter_t;
arbiter_t arbiter;

/* =========================================================================== */


void initialize_arbiter() {
    for (int i=0; i<PHILOSOPHER_COUNT; ++i) {
        arbiter.philosopher_state[i] = 0;       // set everyone to "thinking"
        pthread_cond_init(&arbiter.cond[i], NULL);
    }
    pthread_mutex_init(&arbiter.mutex, NULL);
}

// --- request chopsticks
void request_chopsticks(int philosopher_id, int chopstick_1, int chopstick_2) {
    pthread_mutex_lock(&arbiter.mutex);
    arbiter.philosopher_state[philosopher_id] = 1;  // set to "waiting"

    // wait for both chopsticks
    while (1) {
        // try locking chopsticks
        if (pthread_mutex_trylock(&chopstick[chopstick_1]) == 0) {
            if (pthread_mutex_trylock(&chopstick[chopstick_2]) == 0) {
                break;  // got both chopsticks
            } else {
                // failed second chopstick, release first and wait
                pthread_mutex_unlock(&chopstick[chopstick_1]);
            }
        }
        pthread_cond_wait(&arbiter.cond[philosopher_id], &arbiter.mutex);
    }

    arbiter.philosopher_state[philosopher_id] = 2;  // set to "eating"
    pthread_mutex_unlock(&arbiter.mutex);
}

// --- release chopsticks
void release_chopsticks(int philosopher_id, int chopstick_1, int chopstick_2) {
    pthread_mutex_lock(&arbiter.mutex);

    arbiter.philosopher_state[philosopher_id] = 0;  // set to "thinking"
    pthread_mutex_unlock(&chopstick[chopstick_1]);
    pthread_mutex_unlock(&chopstick[chopstick_2]);

    // signal others to try to eat
    for (int i=0; i<PHILOSOPHER_COUNT; ++i) {
        pthread_cond_signal(&arbiter.cond[i]);
    }
    pthread_mutex_unlock(&arbiter.mutex);
}


/* =========================================================================== */

void *philosopher_program(int philosopher_number) {  
    // the philosopher will think and eat until all the meals are gone

    int how_many_times_did_I_eat = 0;
    int chopstick_1, chopstick_2;

    while (MEALS_ON_TABLE > 0) { 
        // philosophers think before they eat
        //printf("Philosopher %d is thinking\n", philosopher_number);
        usleep(1);  
        
        // philosopher will at least choose two DIFFERENT chopsticks at random
        chopstick_1 = rand() % PHILOSOPHER_COUNT;
        do chopstick_2 = rand() % PHILOSOPHER_COUNT;
        while (chopstick_1 == chopstick_2);
        //printf("Philosopher %d wants chopsticks %d and %d\n", philosopher_number, chopstick_1, chopstick_2);
        request_chopsticks(philosopher_number, chopstick_1, chopstick_2);

        // philosopher eats for twice as much time as thinking
        //printf("Philosopher %d is ready to eat\n", philosopher_number);
        usleep(2);
        
        // put the chopsticks back on the table
        if (MEALS_ON_TABLE > 0) { 
            how_many_times_did_I_eat++;
            pthread_mutex_lock(&MEAL_COUNT_LOCK);
            MEALS_ON_TABLE--;
            printf("Philosopher %d finished eating, %d meals remaining\n", philosopher_number, MEALS_ON_TABLE);
            pthread_mutex_unlock(&MEAL_COUNT_LOCK);
        } else { 
            //printf("Philosopher %d didn't eat because no food was left\n", philosopher_number);
        }
            
        release_chopsticks(philosopher_number, chopstick_1, chopstick_2);
        //printf("Philosopher %d has placed chopsticks on the table\n", philosopher_number);
    } 

    printf("PHILOSOPHER %d GOT TO EAT %d TIMES\n", philosopher_number, how_many_times_did_I_eat);
    return(NULL);
}

/* =========================================================================== */

int main() {  
    int i;

   srand(time(NULL));   // seed the random number generator
   
   // initialize the MEALS_ON_TABLE variable locking mutex
   pthread_mutex_init(&MEAL_COUNT_LOCK, NULL);
   
   // initialize all the chopsticks as "unlocked" or "not held"
   for(i=0; i<PHILOSOPHER_COUNT; i++) {
      pthread_mutex_init(&chopstick[i],NULL); }

   // create the philosopher threads  
   for(i=0; i<PHILOSOPHER_COUNT; i++) {
      pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i); }

   // collect the philosophers when they are done.  
   // if the philosopher threads never terminate, these joins never happen
   for(i=0; i<PHILOSOPHER_COUNT; i++) {
      pthread_join(philosopher[i],NULL); }

   // once all threads terminate, destroy all the chopsticks
   for(i=0; i<PHILOSOPHER_COUNT; i++) {
      pthread_mutex_destroy(&chopstick[i]); }

   return 0;
}




