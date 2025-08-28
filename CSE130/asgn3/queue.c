#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

struct queue {
    void **buffer;
    int in;
    int out;
    int length;
    int counter;
};

pthread_mutex_t L = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_init(&L, NULL);
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
// pthread_cond_init(&full, NULL);
// pthread_cond_init(&empty, NULL);

queue_t *queue_new(int size) {
    queue_t *new = malloc(1 * sizeof(queue_t));
    new->length = size;
    new->buffer = malloc(size * sizeof(void *));
    new->in = 0;
    new->out = 0;
    new->counter = 0;
    return new;
}

void queue_delete(queue_t **q) {
    free((*q)->buffer);
    free(*q);
    *q = NULL;
}

bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }
    pthread_mutex_lock(&L);
    while (q->counter == q->length) {
        pthread_cond_wait(&full, &L);
    }
    q->buffer[q->in] = elem;
    q->in = (q->in + 1) % q->length;
    q->counter += 1;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&L);
    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return false;
    }
    pthread_mutex_lock(&L);
    while (q->counter == 0) {
        pthread_cond_wait(&empty, &L);
    }
    void *item = q->buffer[q->out];
    *elem = item;
    q->out = (q->out + 1) % q->length;
    q->counter -= 1;
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&L);
    return true;
}
