#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <pthread.h>

/* a field for every bookkeeping variable that is needed by your Implementation */
typedef struct RW_lock_s {
    pthread_mutex_t mutex;      // mutex to protect shared state (read_count, write_flag)
    pthread_cond_t read_cond;   // condition variable for readers
    pthread_cond_t write_cond;  // condition variable for writers
    int read_count;             // number of active readers
    int write_flag;             // 1 if a writer holds the lock, 0 otherwise
    int writers_waiting;        // writers waiting for lock
} RW_lock_t;

void RW_lock_init(RW_lock_t *lock);
    /* This routine should be called on a pointer to a struct variable of RW_lock_t to initialize it and ready it for use. */

void RW_read_lock(RW_lock_t *lock);
	/* This routine should be called at the beginning of a READER critical section */

void RW_read_unlock(RW_lock_t *lock);
     /* This routine should be called at the end of a READER critical section */

void RW_write_lock(RW_lock_t *lock);
     /* This routine should be called at the beginning of a WRITER critical section */

void RW_write_unlock(RW_lock_t *lock);
     /* This routine should be called at the end of a WRITER critical section */

#endif // RW_LOCK_H