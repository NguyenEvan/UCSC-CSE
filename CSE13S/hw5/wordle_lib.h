#pragma once

#include <stdbool.h>
#include <stdio.h>

bool score_guess(char *secret, char *guess, char *result);

bool valid_guess(char *guess, char **vocabulary, size_t num_words);

char **load_vocabulary(char *filename, size_t *num_words);

void free_vocabulary(char **vocabulary, size_t num_words);

bool hard_mode_valid_guess(char *secret, char *guess, char **vocabulary, size_t num_words, int* forbidden_x, int* forbidden_y, int* forbidden_g);

void hard_mode_init(int **forbidden_x, int **forbidden_y, int **forbidden_g);

void hard_mode_update_forbidden(char *result, char *guess, char *secret, int *forbidden_x, int *forbidden_y, int *forbidden_g);

void hard_mode_free(int *forbidden_x, int *forbidden_y, int *forbidden_g);
// Idea for the UNUSED macro from Stack Overflow user mtvec. Thanks!
// https://stackoverflow.com/a/3599170
#define UNUSED(x) (void)(x)
