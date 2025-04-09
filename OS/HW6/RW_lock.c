#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include "RW_lock.h"

// --- Initialize the Reader-Writer Lock
void RW_lock_init(RW_lock_t *lock) {
    pthread_mutex_init(&lock->mutex, NULL);         // initialize mutex
    pthread_cond_init(&lock->read_cond, NULL);      // initiailize read contition
    pthread_cond_init(&lock->write_cond, NULL);     // initialize write condition
    lock->read_count = 0;
    lock->write_flag = 0;
    lock->writers_waiting = 0;
}

// --- Read Lock
void RW_read_lock(RW_lock_t *lock) {
    //printf("RW_read_lock\n");
    pthread_mutex_lock(&lock->mutex); // lock mutex
    // wait if writer holds the lock (writer priority)
    while (lock->write_flag || lock->writers_waiting > 0) {
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }
    lock->read_count++;
    pthread_mutex_unlock(&lock->mutex); // release mutex
}

// --- Release Read Lock
void RW_read_unlock(RW_lock_t *lock) {
    //printf("RW_read_unlock\n");
    pthread_mutex_lock(&lock->mutex); // lock mutex
    lock->read_count--;
    // if no more readers, signal waiting writer
    if (lock->read_count == 0) {
        pthread_cond_signal(&lock->write_cond);
    }
    pthread_mutex_unlock(&lock->mutex); // release mutex
}

// --- Writer Lock
void RW_write_lock(RW_lock_t *lock) {
    //printf("RW_write_lock\n");
    pthread_mutex_lock(&lock->mutex); // lock mutex
    lock->writers_waiting++;
    // wait if there are active readers or writer
    while (lock->read_count > 0 || lock->write_flag) {
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }
    lock->writers_waiting--;
    lock->write_flag = 1;
    pthread_mutex_unlock(&lock->mutex); // release mutex
}

// --- Release Write Lock
void RW_write_unlock(RW_lock_t *lock) {
    //printf("RW_write_unlock\n");
    pthread_mutex_lock(&lock->mutex); // lock mutex
    lock->write_flag = 0;
    // (writer priority)
    pthread_cond_signal(&lock->write_cond);     // signal waiting writers first
    pthread_cond_broadcast(&lock->read_cond);   // signal waiting readers
    pthread_mutex_unlock(&lock->mutex); // release mutex
}
