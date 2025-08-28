#include "hashtabledb_lib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void print_customer(hashtable *table, char *email, int pos_in_bucket) {
  unsigned long hashed = hash(email);
  unsigned long bucket_num = hashed % table->num_buckets;
  ll_customer *here = table->buckets[bucket_num];
  for (int i = 0; i < pos_in_bucket; i++){
    here = here->next;
    if (here == NULL){
        printf("Something went very wrong!\n");
    }
  }
  printf("Email: %s\nName: %s\nShoe Size: %d\nFavorite Food: %s\n\n",
         here->cust->email, here->cust->name, here->cust->shoe_size,
         here->cust->fav_food);
}

// From Professor Rudnick's lecture
hashtable *new_hashtable(size_t num_buckets) {
  hashtable *output = calloc(1, sizeof(hashtable));
  output->num_buckets = num_buckets;

  output->buckets = calloc(num_buckets, sizeof(ll_customer *));
  return output;
}

// Inspiration from Professor Rudnick's lecture
void add_customer(hashtable *table, char *email, char *name, int shoe_size,
                  char *fav_food) {
  unsigned long hashed = hash(email);
  unsigned long bucket_num = hashed % table->num_buckets;

  // this is a specific bucket that's in the array of buckets.
  ll_customer *bucket = table->buckets[bucket_num];

  while (bucket != NULL) {
    if (!strcmp(bucket->cust->email, email)) {
      printf("This email was already found. Replacing info with inputted "
             "fields...\n");
      free(bucket->cust->email);
      free(bucket->cust->fav_food);
      free(bucket->cust->name);
      bucket->cust->email = strdup(email);
      bucket->cust->fav_food = strdup(fav_food);
      bucket->cust->name = strdup(name);
      bucket->cust->shoe_size = shoe_size;
      return;
    }
    bucket = bucket->next;
  }

  // allocate the linked list node.
  ll_customer *new_customer = calloc(1, sizeof(ll_customer));
  new_customer->cust = calloc(1, sizeof(Customer));
  new_customer->cust->email = strdup(email);
  new_customer->cust->name = strdup(name);
  new_customer->cust->shoe_size = shoe_size;
  new_customer->cust->fav_food = strdup(fav_food);

  // cons it in there.
  new_customer->next = table->buckets[bucket_num];
  // pop a new pointer into that array of pointers.
  table->buckets[bucket_num] = new_customer;
  return;
}

// Inspiration from Professor Rudnick's lecture
Customer *lookup(hashtable *table, char *email) {
  unsigned long hashed = hash(email);
  unsigned long bucket_num = hashed % table->num_buckets;
  // this is a specific bucket that's in the array of buckets.
  ll_customer *here = table->buckets[bucket_num];
  int pos_in_bucket = 0;

  while (here) {
    if (!strcmp(here->cust->email, email)) {
      print_customer(table, email, pos_in_bucket);
      return here->cust;
    }
    pos_in_bucket++;
    here = here->next;
  }
  printf("No customer with this email was found.\n");
  return NULL;
}

// Inspiration from delete_text() from HW4
void delete_customer(hashtable *table, char *email) {
  unsigned long hashed = hash(email);
  unsigned long bucket_num = hashed % table->num_buckets;
  int pos = 0;
  ll_customer *here_before;
  ll_customer *here = table->buckets[bucket_num];
  
  if (here == NULL){
    printf("No customer with this email was found.\n");
    return;
  }
  ll_customer *here_after = table->buckets[bucket_num]->next;

  while (here) {
    if (!strcmp(here->cust->email, email)) {
      if (pos == 0) {
        table->buckets[bucket_num] = here_after;
      } else {
        here_before->next = here_after;
      }

      free(here->cust->email);
      free(here->cust->fav_food);
      free(here->cust->name);
      free(here->cust);
      free(here);
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

void save_to_db(hashtable *table, char *file) {
  FILE *fptr = fopen(file, "w");
  if (fptr == NULL) {
    printf("Could not open file");
    return;
  }

  for (size_t i = 0; i < table->num_buckets; i++) {
    // Extracts fields from line and adds to hash table
    ll_customer *bucket = table->buckets[i];
    while (bucket != NULL) {
      fprintf(fptr, "%s\t%s\t%d\t%s\n", bucket->cust->email, bucket->cust->name,
              bucket->cust->shoe_size, bucket->cust->fav_food);
      bucket = bucket->next;
    }
  }
  fclose(fptr);
  return;
}

void print_db(hashtable *table) {
  for (size_t i = 0; i < table->num_buckets; i++) {
    ll_customer *bucket = table->buckets[i];
    int pos_in_bucket = 0;
    while (bucket != NULL) {
      print_customer(table, bucket->cust->email, pos_in_bucket);
      pos_in_bucket++;
      bucket = bucket->next;
    }
  }
}

void free_db(hashtable *table) {
  for (size_t i = 0; i < table->num_buckets; i++) {
    ll_customer *bucket = table->buckets[i];
    while (bucket != NULL) {
      delete_customer(table, bucket->cust->email);
      bucket = table->buckets[i];
    }
  }
  free(table->buckets);
  free(table);
}
