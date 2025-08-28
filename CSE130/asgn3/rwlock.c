#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "rwlock.h"

struct rwlock {
    pthread_mutex_t rL;
    pthread_mutex_t wL;
    pthread_mutex_t ruL;
    pthread_mutex_t wuL;
    pthread_cond_t canRead;
    pthread_cond_t canWrite;
    int readers;
    int writers;
    int waiting_readers;
    int waiting_writers;
    PRIORITY priority;
    // pthread_cond_t cond;
    // below are only used if n-way priority
    int n;
    int readers_since_writer;
    //int allowed_readers;
};
#define TRACE true

rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    rwlock_t *new = malloc(1 * sizeof(rwlock_t));
    pthread_mutex_init(&new->rL, NULL);
    pthread_mutex_init(&new->wL, NULL);
    pthread_mutex_init(&new->ruL, NULL);
    pthread_mutex_init(&new->wuL, NULL);
    pthread_cond_init(&new->canRead, NULL);
    pthread_cond_init(&new->canWrite, NULL);
    new->readers = 0;
    new->writers = 0;
    new->waiting_readers = 0;
    new->waiting_writers = 0;
    new->priority = p;
    new->n = n;
    //new->allowed_readers = n;
    new->readers_since_writer = 0;
    return new;
}

void rwlock_delete(rwlock_t **rw) {
    free(*rw);
    *rw = NULL;
}

void reader_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->rL);
    if (TRACE) {
        printf("Locking reader | Active Readers: %d | Active Writers: %d | Waiting Readers: %d | "
               "Waiting Writers: %d\n",
            rw->readers, rw->writers, rw->waiting_readers, rw->waiting_writers);
    }
    rw->waiting_readers += 1;
    if (rw->priority == READERS) {
        while (rw->writers > 0) {
            pthread_cond_wait(&rw->canRead, &rw->rL);
        }
    } else if (rw->priority == WRITERS) {
        while (rw->writers > 0 || rw->waiting_writers > 0) {
            pthread_cond_wait(&rw->canRead, &rw->rL);
        }
    } else {
        while (rw->writers > 0 || (rw->waiting_writers > 0 && rw->readers_since_writer >= rw->n)) {
            pthread_cond_wait(&rw->canRead, &rw->rL);
        }
        rw->readers_since_writer += 1;
    }
    rw->waiting_readers -= 1;
    rw->readers += 1;
    pthread_mutex_unlock(&rw->rL);
}

void reader_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->ruL);
    if (TRACE) {
        printf("UnLocking reader | Active Readers: %d | Active Writers: %d | Waiting Readers: %d | "
               "Waiting Writers: %d\n",
            rw->readers, rw->writers, rw->waiting_readers, rw->waiting_writers);
    }
    rw->readers -= 1;
    if (rw->priority == READERS) {
        if (rw->readers == 0) {
            pthread_cond_signal(&rw->canWrite);
        }
    } else if (rw->priority == WRITERS) {
        if (rw->readers == 0) {
            pthread_cond_signal(&rw->canWrite);
            // if no writers, let them read
            // if (rw->waiting_readers > 0){
            //     pthread_cond_signal(&canRead);
            // }
        }
    } else {
        if (rw->readers == 0) {
            // Suggested changes: waiting_writing > 0 && readers == 0 (&& readers_waiting == 0)
            pthread_cond_signal(&rw->canWrite);
        }
    }
    pthread_mutex_unlock(&rw->ruL);
}

void writer_lock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->wL);
    if (TRACE) {
        printf("Locking writer | Active Readers: %d | Active Writers: %d | Waiting Readers: %d | "
               "Waiting Writers: %d\n",
            rw->readers, rw->writers, rw->waiting_readers, rw->waiting_writers);
    }
    rw->waiting_writers += 1;
    if (rw->priority == WRITERS || rw->priority == READERS) {
        while (rw->readers > 0 || rw->writers > 0) {
            pthread_cond_wait(&rw->canWrite, &rw->wL);
        }
    } else {
        while (rw->readers > 0 || rw->writers > 0 || (rw->readers_since_writer < rw->n && rw->waiting_readers > 0)) {
            // Suggested changes: add "|| rw->waiting_readers > 0"
            pthread_cond_wait(&rw->canWrite, &rw->wL);
        }
    }
    rw->writers += 1;
    rw->waiting_writers -= 1;
    pthread_mutex_unlock(&rw->wL);
}

void writer_unlock(rwlock_t *rw) {
    pthread_mutex_lock(&rw->wuL);
    if (TRACE) {
        printf("UnLocking writer | Active Readers: %d | Active Writers: %d | Waiting Readers: %d | "
               "Waiting Writers: %d\n",
            rw->readers, rw->writers, rw->waiting_readers, rw->waiting_writers);
    }
    rw->writers -= 1;
    if (rw->priority == READERS) {
        if (rw->writers == 0) {
            pthread_cond_broadcast(&rw->canRead);
            if (rw->readers == 0) {
                pthread_cond_signal(&rw->canWrite);
            }
        }
    } else if (rw->priority == WRITERS) {
        if (rw->writers == 0) {
            if (rw->waiting_writers > 0) {
                pthread_cond_signal(&rw->canWrite);
            } else {
                pthread_cond_broadcast(&rw->canRead);
            }
        }
    } else {
        if (rw->writers == 0) {
            rw->readers_since_writer = 0;
            if (rw->waiting_readers > 0) {
                // rw->readers_since_writer = 0;
                pthread_cond_broadcast(&rw->canRead);
            } else if (rw->waiting_writers > 0) {
                pthread_cond_signal(&rw->canWrite);
            }
        }
    }
    pthread_mutex_unlock(&rw->wuL);
}
