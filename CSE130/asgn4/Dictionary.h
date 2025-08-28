#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "rwlock.h"

typedef struct UriLock {
  char *uri;
  rwlock_t *lock;
} UriLock;

typedef struct ll_lock {
  struct UriLock *urilock;
  struct ll_lock *next;
} ll_lock;

typedef struct hashtable {
  // buckets
  size_t num_buckets;
  // array of the linked lists that we have;
  ll_lock **buckets;
} hashtable;

void chomp(char *s);

hashtable *new_hashtable(size_t num_buckets);

void add_urilock(hashtable *table, char *uri, rwlock_t* lock);

rwlock_t *lookup(hashtable *table, char *uri);

void delete_urilock(hashtable *table, char *uri);

void print_urilock(hashtable *table, char *uri, int pos_in_bucket);

void free_db(hashtable *table);
