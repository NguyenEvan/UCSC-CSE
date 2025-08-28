#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hashtabledb_lib.h"

void print_help(void) {
  printf("valid commands are...\n");
  printf("- help: display this message\n");
  printf("- add: add a customer to the hash table\n");
  printf("- lookup: print a given customer's information\n");
  printf("- delete: remove a given customer from the hash table\n");
  printf("- list: display every customer's information\n");
  printf("- save: write the current hash table to the database\n");
  printf("- quit: exit the program\n");
}

bool contains_tab(char *input){
  for (int i = 0; i < (int) strlen(input); i++){
    if (input[i] == '\t'){
      return true;
    }
  }
  return false;
}

bool is_number(char *input){
  for (int i = 0; i < (int) strlen(input); i++){
    if (!(input[i] >= '0' && input[i] <= '9')){
      return false;
    }
  }
  return true;
}

void exit_program(hashtable *table, FILE *fptr){
  free_db(table);
  fclose(fptr);
}

int main(void) {

  printf("**************************************\n");
  printf("* welcome to the cse13s customer database *\n");
  printf("**************************************\n");
  printf("Loading customers.tsv into the hash table...\n");

  const int BUFSIZE = 1024;
  char buf[BUFSIZE];
  char *result;
  FILE *fptr = fopen("customers.tsv", "r");
  hashtable *table = new_hashtable(100);

  char *token;

  while (fgets(buf, 1024, fptr) != NULL) {
    int iter = 1;
    char *email;
    char *name;
    char *shoe_size;
    char *fav_food;
    char *ptr;

    token = strtok(buf, "\t");
    email = token;

    while (token != NULL) {
      token = strtok(NULL, "\t");
      if (token == NULL) {
        break;
      }
      if (iter == 1) {
        name = token;
      } else if (iter == 2) {
        shoe_size = token;
      } else if (iter == 3) {
        chomp(token);
        fav_food = token;
      } else {
        printf("Something went very wrong!\n");
      }
      iter++;
    }
    add_customer(table, email, name, strtol(shoe_size, &ptr, 10), fav_food);
  }

  while (true) {
    printf("command (Type \"help\" for list of commands): ");
    result = fgets(buf, BUFSIZE, stdin);
    if (!result) {
      break;
    }
    chomp(result);

    if (!strcmp(buf, "quit") || !strcmp(buf, "q")) {
      break;
    } else if (!strcmp(buf, "help") || !strcmp(buf, "?")) {
      print_help();
    } else if (!strcmp(buf, "list")) {
      print_db(table);
    } else if (!strcmp(buf, "save")) {
      save_to_db(table, "customers.tsv");
      printf("saved to customers.tsv\n");
    } else if (!strcmp(buf, "add")) {
      char email[1024];
      char name[1024];
      char shoe_size[1024];
      char fav_food[1024];
      char *ptr;
      printf("Adding a new customer to the database...\n");
      printf("Email Address? ");
      if (fgets(email, 1024, stdin) == NULL) {
        break;
      }
      chomp(email);
      while (contains_tab(email)){
        printf("Your input contains a tab.\n");
        printf("Email Address? ");
        if (fgets(email, 1024, stdin) == NULL) {
          exit_program(table, fptr);
          return 0;
        }
        chomp(email);
      }

      printf("Name? ");
      if (fgets(name, 1024, stdin) == NULL) {
        break;
      }
      chomp(name);
      while (contains_tab(name)){
        printf("Your input contains a tab.\n");
        printf("Name? ");
        if (fgets(name, 1024, stdin) == NULL) {
          exit_program(table, fptr);
          return 0;
        }
        chomp(name);
      }

      printf("Shoe Size? ");
      if (fgets(shoe_size, 1024, stdin) == NULL) {
        break;
      }
      chomp(shoe_size);
      while (contains_tab(shoe_size) || !is_number(shoe_size)){
        printf("Your input contains a tab or isn't a number.\n");
        printf("Shoe Size? ");
        if (fgets(shoe_size, 1024, stdin) == NULL) {
          exit_program(table, fptr);
          return 0;
        }
        chomp(shoe_size);
      }

      printf("Favorite Food? ");
      if (fgets(fav_food, 1024, stdin) == NULL) {
        break;
      }
      chomp(fav_food);
      while (contains_tab(fav_food)){
        printf("Your input contains a tab.\n");
        printf("Favorite Food? ");
        if (fgets(fav_food, 1024, stdin) == NULL) {
          exit_program(table, fptr);
          return 0;
        }
        chomp(fav_food);
      }
      add_customer(table, email, name, strtol(shoe_size, &ptr, 10), fav_food);
    } else if (!strcmp(buf, "delete")) {
      char email[1024];
      printf("Deleting a customer from the database...\n");
      printf("Email Address? ");
      if (fgets(email, 1024, stdin) == NULL) {
        break;
      }
      chomp(email);
      delete_customer(table, email);
    } else if (!strcmp(buf, "save")) {
      save_to_db(table, "customers.tsv");
    } else if (!strcmp(buf, "lookup")) {
      char email[1024];
      printf("Email Address? ");
      if (fgets(email, 1024, stdin) == NULL) {
        break;
      }
      chomp(email);
      lookup(table, email);
    } else {
      printf("unknown command: %s\n", buf);
    }
  }
  // done with the loop. Clean up by undoing until the hash table is empty.
  free_db(table);
  fclose(fptr);

  printf("bye!\n");
  return 0;
}
