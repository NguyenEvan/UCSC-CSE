/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA1 
* Lex.c
* Client for the List ADT; Used to lexicographically sort strings
***/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "List.h"

#define MAX_LEN 300

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int line_count;
    FILE *in, *out;
    char line[MAX_LEN];
    
     // open files for reading and writing 
    in = fopen(argv[1], "r");
    if( in==NULL ){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    // read each line of input file, then count and print tokens
    line_count = 0;
    char **arr = NULL;
    while( fgets(line, MAX_LEN, in) != NULL)  {
        line_count += 1;
        arr = realloc(arr, line_count * sizeof(char *));
        if (strlen(line) > 0 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';  // Replace newline with null terminator
        }
        arr[line_count - 1] = calloc(strlen(line) + 1, sizeof(char));
        strcpy(arr[line_count - 1], line);
        
    }
    if (arr == NULL){
        return 0;
    }
    List L = newList();
    append(L, 0);
    moveFront(L);
    for (int i = 1; i < line_count; i++){
        if (strcmp(arr[i], arr[get(L)]) < 0){
            prepend(L, i);
            moveFront(L);
        }
        else if(strcmp(arr[i], arr[get(L)]) == 0){
            insertAfter(L, i);
            moveFront(L);
        }
        else{
            moveBack(L);
            while(index(L)>=0 && strcmp(arr[i], arr[get(L)]) < 0){
                movePrev(L);
            }
            if (index(L) == -1){
                prepend(L, i);
            }
            else{
                insertAfter(L, i);
            }
            moveFront(L);
        }

    }
    for (int i = 1; i < line_count; i++){
        fprintf(out, "%s\n", arr[get(L)]);
        moveNext(L);
    }
    fprintf(out, "%s", arr[get(L)]);
    freeList(&L);
    for (int i = 0; i < line_count; i++){
        free(arr[i]);
    }
    free(arr);
    fclose(in);
    fclose(out);
    return 0;
}