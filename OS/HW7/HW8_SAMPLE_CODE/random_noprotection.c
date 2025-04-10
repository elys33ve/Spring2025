#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* =========================================================================== */

#define PHILOSOPHER_COUNT 10
#define AVAILABLE_MEALS 10000000
pthread_t philosopher[PHILOSOPHER_COUNT];
pthread_mutex_t chopstick[PHILOSOPHER_COUNT];

int MEALS_ON_TABLE = AVAILABLE_MEALS;
pthread_mutex_t MEAL_COUNT_LOCK;

/* =========================================================================== */

void *philosopher_program(int philosopher_number) {  
    // the philosopher will think and eat until all the meals are gone

    int how_many_times_did_I_eat = 0;
    int chopstick_1;
    int chopstick_2;
    
    while (MEALS_ON_TABLE > 0) { 
        // philosophers think before they eat
        printf ("Philosopher %d is thinking\n", philosopher_number);
        usleep(1);  
        
        // philosopher will at least choose two DIFFERENT chopsticks at random
        chopstick_1 = rand() % PHILOSOPHER_COUNT;
        do chopstick_2 = rand() % PHILOSOPHER_COUNT;
        while (chopstick_1 == chopstick_2);
        
        printf ("Philosopher %d wants chopsticks %d and %d\n", philosopher_number, chopstick_1, chopstick_2);
        pthread_mutex_lock(&chopstick[chopstick_1]);
        pthread_mutex_lock(&chopstick[chopstick_2]);

        // philosopher eats for twice as much time as thinking
        printf ("Philosopher %d is ready to eat\n", philosopher_number);
        usleep(2);
        
        // put the chopsticks back on the table
        if (MEALS_ON_TABLE > 0) { 
            how_many_times_did_I_eat++;
            pthread_mutex_lock(&MEAL_COUNT_LOCK);
            MEALS_ON_TABLE--;
            printf ("Philosopher %d finished eating, %d meals remaining\n", philosopher_number, MEALS_ON_TABLE);
            pthread_mutex_unlock(&MEAL_COUNT_LOCK);
        } else { 
            printf("Philosopher %d didn't eat because no food was left\n", philosopher_number);
        }
            
        pthread_mutex_unlock(&chopstick[chopstick_1]);
        pthread_mutex_unlock(&chopstick[chopstick_2]);
        printf("Philosopher %d has placed chopsticks on the table\n", philosopher_number);
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


