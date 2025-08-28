/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA2
* FindPath.c
* Client for the Graph ADT; Used to find the distance and a path between two vertices
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
    
    while( !finishedReading && fgets(line, MAX_LEN, in) != NULL)  {
        ptr = line;
        while (*ptr != '\n') {
            source = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            destination = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            // printf("source: %d, destination: %d\n", source, destination);
            if (source == 0 && destination == 0){
                finishedReading = true;
            }
            else{
                // printf("source: %d\n", source);
                // printf("destination: %d\n", destination);
                addEdge(G, source, destination);
            }
        }
    }
    printGraph(out, G);
    fprintf(out, "\n");
    finishedReading = false;
    while(!finishedReading && fgets(line, MAX_LEN, in) != NULL)  {
        ptr = line;
        while (*ptr != '\n') {
            source = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            destination = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            if (source == 0 && destination == 0){
                finishedReading = true;
                break;
            }
            else{
                BFS(G, source);
                int distance = getDist(G, destination);
                if (distance == INF){
                    fprintf(out, "The distance from %d to %d is infinity\n", source, destination);
                    fprintf(out, "No %d-%d path exists\n", source, destination);
                    fprintf(out, "\n");
                }
                else{
                    fprintf(out, "The distance from %d to %d is %d\n", source, destination, distance);
                    fprintf(out, "A shortest %d-%d path is: ", source, destination);
                    List path = newList();
                    getPath(path, G, destination);
                    printList(out, path);
                    fprintf(out, "\n\n");
                    freeList(&path);
                }
            }
        }
    }
   
    freeGraph(&G);
    fclose(in);
    fclose(out);
    return 0;
}