#include "wordle_lib.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Most of the code that you're going to have to implement goes in here.
// Feel free to add more helper functions if necessary.

// Find the first newline in the input and replace it with \0
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


// Returns true if the guess is an exact match with the secret word, but
// more importantly, fills in the result with the following:
// - 'x' goes in a slot if the corresponding letter in the guess does not appear
//   anywhere in the secret word
// - 'y' goes in a slot if the corresponding letter in the guess appears in the
//   secret word, but not in the corresponding slot
// - 'g' goes in a slot if the corresponding letter in the guess appears in the
//   corresponding slot in the secret word.
//   You can assume that result points at enough memory for a string of length
//   5. (ie, at least 6 bytes long)
bool score_guess(char *secret, char *guess, char *result) {
  // TODO(you): finish this function
  for (int i = 0; i < (int) strlen(guess); i++){
    char letter = guess[i];
    int found = 0;
    if (letter == secret[i]){
      found = 1;
      result[i] = 'g';
    }
    else{
      for (int j = 0; j < (int) strlen(secret); j++){
        if (letter == secret[j]){
          found = 1;
          result[i] = 'y';
        }
      }
    }

    if (!found){
      result[i] = 'x';
    }
  }

  if (strcmp(result, "ggggg") == 0){
    return true;
  }
  return false;
}

// Returns true if the specified guess is one of the strings in the vocabulary,
// and false otherwise. num_words is the length of the vocabulary.
// A simple linear scan over the strings in vocabulary is fine for our purposes,
// but consider: could you do this search more quickly?
bool valid_guess(char *guess, char **vocabulary, size_t num_words) {
  // TODO(you): finish this function
  for (size_t i = 0; i < num_words; i++){
    if (strcmp(guess, vocabulary[i]) == 0){
      return true;
    }
  }
  return false;
}

// Returns an array of strings (so, char **), where each string contains a word
// from the specified file. The file is assumed to contain 5-letter words, one
// per line.
// Also, this function sets the value pointed at by *num_words to be the number
// of words read.
// This will need to allocate enough memory to hold all of the char* pointers --
// so you will keep track of the size of your char** array and then use realloc
// to make the array larger over time, so that you have enough space for the
// dynamically-growing array of char *.
// Use fopen to open the input file for reading,
// strdup (or strndup) to make copies of each word read from that file, and
// fclose to close the file when you are done reading from it.
// Each element of the array should be a single five-letter word,
// null-terminated.
char **load_vocabulary(char *filename, size_t *num_words) {
  char **out = NULL;
  // TODO(you): finish this function
  FILE *fp;
  fp = fopen(filename, "r");
  char buffer[1024];
  int words = 0;
  while(fgets(buffer, 1024, fp) != NULL){
    words++;
    out = realloc(out, sizeof(char*) * words);
    chomp(buffer);
    out[words - 1] = strdup(buffer);
    // printf("Strlen: %d", (int) strlen(out[words - 1]));
  }
  fclose(fp);
  *num_words = words;

  return out;
}

// Free each of the strings in the vocabulary, as well as the pointer vocabulary
// itself (which points to an array of char *).
void free_vocabulary(char **vocabulary, size_t num_words) {
  // TODO(you): finish this function
  for (size_t i = 0; i < num_words; i++){
    free(vocabulary[i]);
  }
  free(vocabulary);
}

bool hard_mode_valid_guess(char *secret, char *guess, char **vocabulary, size_t num_words, int* forbidden_x, int* forbidden_y, int* forbidden_g){
  bool in_vocab_list = valid_guess(guess, vocabulary, num_words);
  if (!in_vocab_list){
    return false;
  }
  for (int i = 0; i < (int) strlen(guess); i++){
    char current_letter = guess[i];
    if (forbidden_x[current_letter - 97] != -1){
      printf("Letter %c is not allowed\n", current_letter);
      return false;
    }
  }

  for (int i = 0; i < 5; i++){
    int current_letter = forbidden_y[i];
    if (current_letter != -1){
      int found = 0;
      for (int j = 0; j < 5; j++){
        if (secret[i] == guess[j]){
          found = 1;
        }
      }
      if (!found){
        printf("Letter %c needs to be in your word\n", secret[i]);
        return false;
      }
    }
  }

  for (int i = 0; i < 5; i++){
    int current_letter = forbidden_g[i];
    if (current_letter != -1){
      if (secret[i] != guess[i]){
        printf("Letter %c needs to be in your word at position %d\n", secret[i], i + 1);
        return false;
      }
    }
  }

  return true;

}

void hard_mode_init(int **forbidden_x, int **forbidden_y, int **forbidden_g){
  *forbidden_x = calloc(26, sizeof(int));
  *forbidden_y = calloc(5, sizeof(int));
  *forbidden_g = calloc(5, sizeof(int));

  for (int i = 0; i < 5; i++){
    (*forbidden_y)[i] = -1;
    (*forbidden_g)[i] = -1;
  }
  for (int j = 0; j < 26; j++){
    (*forbidden_x)[j] = -1;
  }

  
}

void hard_mode_update_forbidden(char *result, char *guess, char *secret, int *forbidden_x, int *forbidden_y, int *forbidden_g){
  for (int i = 0; i < 5; i++){
    if (result[i] == 'x'){
      forbidden_x[guess[i] - 97] = 1;
    }
    if (result[i] == 'y'){
      int found_position = -1;
      for (int j = 0; j < 5; j++){
        if (guess[i] == secret[j]){
          found_position = j;
        }
      }
      if (found_position == -1){
        printf("Something went very wrong!\n");
        return;
      }
      forbidden_y[found_position] = 1;
    }
    if (result[i] == 'g'){
      forbidden_g[i] = 1;
    }
  }
}

void hard_mode_free(int *forbidden_x, int *forbidden_y, int *forbidden_g){
  free(forbidden_x);
  free(forbidden_y);
  free(forbidden_g);
}
