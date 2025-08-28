/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA3
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
    Graph G = newGraph(10);
    addArc(G, 1, 2);
    addArc(G, 1, 4);
    addArc(G, 4, 3);
    addArc(G, 2, 3);
    addArc(G, 10, 1);
    addArc(G, 8, 2);
    addArc(G, 7, 4);
    addArc(G, 5, 2);
    addArc(G, 2, 5);
    addArc(G, 2, 6);
    addArc(G, 5, 4);
    addArc(G, 9, 10);
    addArc(G, 6, 8);
    addArc(G, 3, 6);
    addArc(G, 5, 7);
    addArc(G, 6, 9);
    printGraph(stdout, G);

    List S = newList();
    for (int i = 1; i <= 10; i++){
        append(S, i);
    }
    DFS(G, S);
    printf("Parent: %d\n", getParent(G, 4));
    printf("Parent: %d\n", getParent(G, 7));
    printf("Parent: %d\n", getParent(G, 2));
    printf("Parent: %d\n", getParent(G, 1));
    printf("Order: %d\n", getOrder(G));
    printf("Size: %d\n", getSize(G));
    printf("Finish: %d\n", getFinish(G, 2));
    printf("Discover: %d\n", getDiscover(G, 2));
    printList(stdout, S);
    printf("\n");
    Graph T = transpose(G);
    Graph C = copyGraph(G);
    printGraph(stdout, T);
    printGraph(stdout, C);
    printf("\n");
    addArc(G, 2, 4);
    addEdge(G, 10, 3);
    printf("Size: %d\n", getSize(G));
    printGraph(stdout, G);
    
    DFS(T, S);
    printf("Parent: %d\n", getParent(T, 4));
    printf("Parent: %d\n", getParent(T, 7));
    printf("Parent: %d\n", getParent(T, 2));
    printf("Parent: %d\n", getParent(T, 1));
    printf("Order: %d\n", getOrder(T));
    printf("Size: %d\n", getSize(T));
    printf("Finish: %d\n", getFinish(T, 2));
    printf("Discover: %d\n", getDiscover(G, 2));
    printList(stdout, S);
    printf("\n");
    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    freeGraph(&C);

    

    return(0);
}