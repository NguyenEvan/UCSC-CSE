/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA2 
* GraphTest.c
* Testing file for Graph ADT 
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"
#include "Graph.h"

// From ListClient.c ---------------------------
int main(int argc, char* argv[]){
    Graph G = newGraph(4);
    printGraph(stdout, G);
    addEdge(G, 1, 2);
    printGraph(stdout, G);
    addEdge(G, 1, 4);
    printGraph(stdout, G);
    addEdge(G, 4, 3);
    addEdge(G, 2, 3);
    printGraph(stdout, G);

    BFS(G, 1);
    printf("Parent: %d\n", getParent(G, 4));
    printf("Parent: %d\n", getParent(G, 3));
    printf("Parent: %d\n", getParent(G, 2));
    printf("Parent: %d\n", getParent(G, 1));
    printf("Order: %d\n", getOrder(G));
    printf("Size: %d\n", getSize(G));
    printf("Source: %d\n", getSource(G));
    printf("Dist: %d\n", getDist(G, 4));
    List L = newList();
    getPath(L, G, 4);
    printList(stdout, L);
    printf("\n");
    addEdge(G, 2, 4);
    printf("Size: %d\n", getSize(G));
    printGraph(stdout, G);
    clear(L);
    makeNull(G);
    addEdge(G, 1, 3);
    BFS(G, 1);
    printGraph(stdout, G);
    getPath(L, G, 2);
    printList(stdout, L);
    printf("\n");
    freeList(&L);
    freeGraph(&G);

    

    return(0);
}