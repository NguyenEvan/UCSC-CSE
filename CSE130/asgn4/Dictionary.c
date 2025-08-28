#include "Dictionary.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rwlock.h"

// Thank you Dan Bernstein.
// From Professor Rudnick's lecture
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while (*str != '\0') {
    c = *str;
    hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
    str++;
  }
  return hash;
}

// From previous homeworks
void chomp(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\n' || s[i] == '\r') {
      // Check for \r in case we're on Windows or a very old Mac??
      // Look, I won't stop you from doing CSE 13s on a Mac from 2001 or prior.
      s[i] = '\0';
      break;
    }
  }
}

// From Professor Rudnick's lecture
hashtable *new_hashtable(size_t num_buckets) {
  hashtable *output = calloc(1, sizeof(hashtable));
  output->num_buckets = num_buckets;

  output->buckets = calloc(num_buckets, sizeof(ll_lock *));
  return output;
}

// Inspiration from Professor Rudnick's lecture
void add_urilock(hashtable *table, char* uri, rwlock_t* lock) {
  unsigned long hashed = hash(uri);
  unsigned long bucket_num = hashed % table->num_buckets;

  // this is a specific bucket that's in the array of buckets.
  ll_lock *bucket = table->buckets[bucket_num];

  while (bucket != NULL) {
    if (!strcmp(bucket->urilock->uri, uri)) {
      printf("This uri was already found. Replacing info with inputted "
             "fields...\n");
      // free? 
      bucket->urilock->uri = strdup(uri);
      bucket->urilock->lock = lock;
      return;
    }
    bucket = bucket->next;
  }

  // allocate the linked list node.
  ll_lock *new_lock = calloc(1, sizeof(ll_lock));
  new_lock->urilock = calloc(1, sizeof(UriLock));
  new_lock->urilock->uri = strdup(uri);
  new_lock->urilock->lock = lock;

  // cons it in there.
  new_lock->next = table->buckets[bucket_num];
  // pop a new pointer into that array of pointers.
  table->buckets[bucket_num] = new_lock;
  return;
}

// Inspiration from Professor Rudnick's lecture
rwlock_t *lookup(hashtable *table, char *uri) {
  unsigned long hashed = hash(uri);
  unsigned long bucket_num = hashed % table->num_buckets;
  // this is a specific bucket that's in the array of buckets.
  ll_lock *here = table->buckets[bucket_num];

  while (here) {
    if ((strcmp(here->urilock->uri, uri) == 0)) {
      // print_customer(table, email, pos_in_bucket);
      return here->urilock->lock;
    }
    here = here->next;
  }
  return NULL;
}

// Inspiration from delete_text() from HW4
void delete_urilock(hashtable *table, char *uri) {
  unsigned long hashed = hash(uri);
  unsigned long bucket_num = hashed % table->num_buckets;
  int pos = 0;
  ll_lock *here_before;
  ll_lock *here = table->buckets[bucket_num];
  
  if (here == NULL){
    printf("No uri was found.\n");
    return;
  }
  ll_lock *here_after = table->buckets[bucket_num]->next;

  while (here) {
    if (!strcmp(here->urilock->uri, uri)) {
      if (pos == 0) {
        table->buckets[bucket_num] = here_after;
      } else {
        here_before->next = here_after;
      }

      // free?
      return;
    }
    here_before = here;
    here = here->next;
    if (here == NULL){
        printf("No customer with this email was found.\n");
        return;
    }
    here_after = here_after->next;
    pos++;
  }
  printf("No customer with this email was found.\n");
  return;
}



void free_db(hashtable *table) {
  for (size_t i = 0; i < table->num_buckets; i++) {
    ll_lock *bucket = table->buckets[i];
    while (bucket != NULL) {
      delete_urilock(table, bucket->urilock->uri);
      bucket = table->buckets[i];
    }
  }
  free(table->buckets);
  free(table);
}
