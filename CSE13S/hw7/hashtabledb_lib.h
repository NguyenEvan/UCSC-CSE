#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Customer {
  char *email;
  char *name;
  int shoe_size;
  char *fav_food;
} Customer;

typedef struct ll_customer {
  Customer *cust;
  struct ll_customer *next;
} ll_customer;

typedef struct hashtable {
  // buckets
  size_t num_buckets;
  // array of the linked lists that we have;
  ll_customer **buckets;
} hashtable;

void chomp(char *s);

hashtable *new_hashtable(size_t num_buckets);

void add_customer(hashtable *table, char *email, char *name, int shoe_size,
                  char *fav_food);

Customer *lookup(hashtable *table, char *email);

void delete_customer(hashtable *table, char *email);

void print_customer(hashtable *table, char *email, int pos_in_bucket);

void save_to_db(hashtable *table, char *file);

void print_db(hashtable *table);

void free_db(hashtable *table);
