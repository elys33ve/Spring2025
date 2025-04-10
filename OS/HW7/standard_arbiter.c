#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
standard_arbiter.c:  
    modified version of standard_noprotection.c that implements the arbiter (waiter) solution.

Arbiter Solution:
Guarantee that a philosopher can only pick up both forks or none by introducing an arbitrator / waiter
to replace circular waiting. To pick up the forks, a philosopher must ask permission of the waiter. 
The waiter gives permission to only one philosopher at a time until the philosopher has picked up 
both of his forks. Putting down a fork is always allowed. The waiter can be implemented as a mutex. 
If a philosopher is eating and one of his neighbors is requesting the forks, all other philosophers 
must wait until this request has been fulfilled even if forks for them are still available.

gcc -o standard_arbiter standard_arbiter.c && ./standard_arbiter
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
void request_chopsticks(int philosopher_id) {
    pthread_mutex_lock(&arbiter.mutex);

    arbiter.philosopher_state[philosopher_id] = 1;  // set to "waiting"
    int left = (philosopher_id + PHILOSOPHER_COUNT - 1) % PHILOSOPHER_COUNT;
    int right = (philosopher_id + 1) % PHILOSOPHER_COUNT;

    // wait for available chopsticks
    while (arbiter.philosopher_state[philosopher_id] == 1 &&
           (arbiter.philosopher_state[left] == 2 || arbiter.philosopher_state[right] == 2)) {
        pthread_cond_wait(&arbiter.cond[philosopher_id], &arbiter.mutex);
    }

    arbiter.philosopher_state[philosopher_id] = 2;  // set to "eating"
    pthread_mutex_unlock(&arbiter.mutex);

    // after getting permission, philosopher acquires the chopsticks
    pthread_mutex_lock(&chopstick[philosopher_id]);
    pthread_mutex_lock(&chopstick[(philosopher_id + 1) % PHILOSOPHER_COUNT]);
}

// --- release chopsticks
void release_chopsticks(int philosopher_id) {
    pthread_mutex_lock(&arbiter.mutex);

    // release chopsticks
    pthread_mutex_unlock(&chopstick[philosopher_id]);
    pthread_mutex_unlock(&chopstick[(philosopher_id + 1) % PHILOSOPHER_COUNT]);
    arbiter.philosopher_state[philosopher_id] = 0;  // set to "thinking"

    // signal neighbors that chopsticks might be available
    int left = (philosopher_id + PHILOSOPHER_COUNT - 1) % PHILOSOPHER_COUNT;
    int right = (philosopher_id + 1) % PHILOSOPHER_COUNT;
    pthread_cond_signal(&arbiter.cond[left]);
    pthread_cond_signal(&arbiter.cond[right]);

    pthread_mutex_unlock(&arbiter.mutex);
}

/* =========================================================================== */

void *philosopher_program(int philosopher_number) {  
    // the philosopher will think and eat until all the meals are gone
    int how_many_times_did_I_eat = 0;
      
    while (MEALS_ON_TABLE > 0) { 
        // philosophers always think before they eat
        //printf("Philosopher %d is thinking\n", philosopher_number);
        usleep(1);  
        
        // after thinking, asks permission to grab chopsticks
        // grabs the chopsticks to the right and left.
        // left of philosopher N = chopstick N, right = chopstick N+1
        //printf("Philosopher %d wants chopsticks\n", philosopher_number);
        request_chopsticks(philosopher_number);

        // philosopher eats for twice as much time eating as thinking
        //printf("Philosopher %d is ready to eat\n", philosopher_number);
        usleep(2);
        
        // put the chopsticks back on the table
        if (MEALS_ON_TABLE > 0) { 
            how_many_times_did_I_eat++;
            pthread_mutex_lock(&MEAL_COUNT_LOCK);
            MEALS_ON_TABLE--;
            printf("Philosopher %d finished eating, %d meals remaining\n",philosopher_number, MEALS_ON_TABLE);
            pthread_mutex_unlock(&MEAL_COUNT_LOCK);
        } else { 
            //printf("Philosopher %d didn't eat because no food was left\n", philosopher_number);
        }

        release_chopsticks(philosopher_number);
        //printf("Philosopher %d has placed chopsticks on the table\n", philosopher_number);
    } 

    printf("PHILOSOPHER %d GOT TO EAT %d TIMES\n", philosopher_number, how_many_times_did_I_eat);
    return(NULL);
}

/* =========================================================================== */

int main() {  
    int i;

    // initialize arbiter and the MEALS_ON_TABLE variable locking mutex
    initialize_arbiter();
    pthread_mutex_init(&MEAL_COUNT_LOCK, NULL);
   
    // initialize all the chopsticks as "not held"
    for (i=0; i<PHILOSOPHER_COUNT; i++) {
        pthread_mutex_init(&chopstick[i],NULL); }

    // create the philosopher threads  
    for (i=0; i<PHILOSOPHER_COUNT; i++) {
        pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i); 
    }

    // collect the philosophers when they are done.
    // if the philosopher threads never terminate, these joins never happen
    for (i=0; i<PHILOSOPHER_COUNT; i++) {
        pthread_join(philosopher[i],NULL); }

    // once all the threads terminate, destroy all the chopsticks
    for (i=0; i<PHILOSOPHER_COUNT; i++) {
        pthread_mutex_destroy(&chopstick[i]); }

    return 0;
}





