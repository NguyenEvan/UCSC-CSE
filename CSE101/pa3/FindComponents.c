/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA3
* FindComponents.c
* Client for the Graph ADT; Used to find the strongly components of a graph using depth-first search
***/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "List.h"
#include "Graph.h"

#define MAX_LEN 300

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *in, *out;
    char line[MAX_LEN];
    char *endptr;
    char *ptr;
    int source;
    int destination;
    bool finishedReading = false;
    
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

    if (fgets(line, MAX_LEN, in) == NULL){
        printf("Empty file is unable to be read\n");
        exit(EXIT_FAILURE);
    }

    Graph G = newGraph((int) strtol(line, &endptr, 10));
    
    while(!finishedReading && fgets(line, MAX_LEN, in) != NULL)  {
        ptr = line;
        while (*ptr != '\n' && *ptr != ' ') {
            source = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            destination = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            if (source == 0 && destination == 0){
                finishedReading = true;
                break;
            }
            else{
                addArc(G, source, destination);
            }
        }
    }
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    List S = newList();
    for(int i=1; i<=getOrder(G); i++) append(S, i);
    DFS(G, S);
    Graph G_T = transpose(G);
    DFS(G_T, S);
    List O = newList();
    int comp = 0;
    for (moveBack(S); index(S) >= 0; movePrev(S)){
        if (getParent(G_T, get(S)) == NIL){
            comp += 1;
        }
    }
    fprintf(out, "G contains %d strongly connected components:\n", comp);
    comp = 1;
    for (moveBack(S); index(S) >= 0; movePrev(S)){
        // printf("Element: %d\n", get(S));
        // printf("Parent: %d\n", getParent(G, get(S)));
        if (getParent(G_T, get(S)) == NIL){
            prepend(O, get(S));
            fprintf(out, "Component %d: ", comp);
            printList(out, O);
            fprintf(out, "\n");
            clear(O);
            comp += 1;
        }
        else{
            prepend(O, get(S));
        }
    }
    
   
    freeGraph(&G);
    freeGraph(&G_T);
    freeList(&S);
    fclose(in);
    freeList(&S);
    fclose(out);
    return 0;
}