#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* a field for every bookkeeping variable that is needed by your Implementation */
typedef struct RW_lock_s {
    pthread_mutex_t mutex;      // Protects the shared state (read_count, write_flag)
    pthread_cond_t read_cond;   // Condition variable for readers
    pthread_cond_t write_cond;  // Condition variable for writers
    int read_count;             // Number of active readers
    int write_flag;             // 1 if a writer holds the lock, 0 otherwise
} RW_lock_t;



// --- Initialize the Reader-Writer Lock
void RW_lock_init(RW_lock_t *lock) {
    // initialize mutex
    if (pthread_mutex_init(&lock->mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return;
    }
    // initiailize read contition
    if (pthread_cond_init(&lock->read_cond, NULL) != 0) {
        perror("Read condition variable initialization failed");
        pthread_mutex_destroy(&lock->mutex);
        return;
    }
    // initialize write condition
    if (pthread_cond_init(&lock->write_cond, NULL) != 0) {
        perror("Write condition variable initialization failed");
        pthread_mutex_destroy(&lock->mutex);
        pthread_cond_destroy(&lock->read_cond);
        return;
    }
    lock->read_count = 0;   // set read count zero
    lock->write_flag = 0;   // set writer lock zero
}

// --- Acquire Read Lock
void RW_read_lock(RW_lock_t *lock) {
    // acquire mutex lock
    pthread_mutex_lock(&lock->mutex);
    while (lock->write_flag) {
        // wait if writer holds the lock
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }
    lock->read_count++;     // update reader count
    pthread_mutex_unlock(&lock->mutex);     // release lock
}

// --- Release Read Lock
void RW_read_unlock(RW_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->read_count--;     // update reader count
    printf("read unlock\n");

    // if no more readers, signal waiting writer
    if (lock->read_count == 0) {
        pthread_cond_signal(&lock->write_cond);
    }
    pthread_mutex_unlock(&lock->mutex);     // release lock
}

// --- Acquire Writer Lock
void RW_write_lock(RW_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->read_count > 0 || lock->write_flag) {
        // wait if there are active readers or a writer holds the lock
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }
    lock->write_flag = 1;
    pthread_mutex_unlock(&lock->mutex);     // release lock
}

// --- Release Write Lock
void RW_write_unlock(RW_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->write_flag = 0;
    // Signal waiting writers first (writer priority - can be changed for reader priority)
    pthread_cond_signal(&lock->write_cond);
    // Then signal waiting readers
    pthread_cond_broadcast(&lock->read_cond);
    
    pthread_mutex_unlock(&lock->mutex);     // release lock
}
