/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA3
* Graph.c
* C file for Graph ADT 
***/ 

//-----------------------------------------------------------------------------
// Graph.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <string.h>
#include<stdbool.h>
#include "Graph.h"
#include "List.h"

// structs --------------------------------------------------------------------

// private GraphObj type
typedef struct GraphObj{
   List* adjLists; // array of Lists whose 𝑖th element contains the neighbors of vertex 𝑖. 
   int* color; // array of ints whose ith element is the color of vertex i
               // 0: white, 1: gray, 2: black
   int* parent; // array of ints whose ith element is the parent of vertex i
   int* discover; // array of ints whose ith element is the discover time of its respective element
   int* finish; // array of ints whose ith element is the finish time of its respective element
   int order; // number of vertices
   int size; // number of edges
} GraphObj;

// Constructors-Destructors

// newGraph()
// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges.
Graph newGraph(int n){
    Graph G = malloc(sizeof(GraphObj));
    G->order = n;
    G->size = 0;
    G->adjLists = malloc(sizeof(List) * (n+1));
    G->color = malloc(sizeof(int) * (n+1));
    G->parent = malloc(sizeof(int) * (n+1));
    G->discover = malloc(sizeof(int) * (n+1));
    G->finish = malloc(sizeof(int) * (n+1));
    G->adjLists[0] = newList();
    for (int i = 1; i < n + 1; i++){
        G->parent[i] = G->color[i] = NIL;
        G->discover[i] = G->finish[i] = UNDEF;
        G->adjLists[i] = newList();
    }
    return G;
}

// freeGraph()
// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL.
void freeGraph(Graph* pG){
    if( pG!=NULL && *pG!=NULL ){
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->discover);
        free((*pG)->finish);
        for (int i = 0; i <= (*pG)->order; i++){
            freeList(&((*pG)->adjLists[i]));
        }
        free((*pG)->adjLists);
        free(*pG);
        *pG = NULL;
   }
}

// Access functions

// getOrder()
// returns the order of the graph (number of vertices)
int getOrder(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getOrder() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->order;
}

// getSize()
// returns the size of the graph (number of edges)
int getSize(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getSize() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->size;
}

// getParent()
// return the parent of vertex u in the DFS forest created by DFS(), or NIL if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G) 
int getParent(Graph G, int u){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getParent() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling getParent() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->parent[u];
}

// getDiscover()
// returns the discover time of vertex u in the DFS forest created by DFS(), or UNDEF if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G) 
int getDiscover(Graph G, int u){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getDiscover() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling getDiscover() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->discover[u];
}

// getFinish()
// returns the finish time of vertex u in the DFS forest created by DFS(), or UNDEF if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G)
int getFinish(Graph G, int u){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getFinish() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling getFinish() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->finish[u];
} 
// Manipulation procedures

// addArc()
// inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣)
// Pre: 1<=u<=n, 1<=v<=n
void addArc(Graph G, int u, int v){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling addArc() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling addArc() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if (v < 1 || v > getOrder(G)){
        fprintf(stderr, "Graph Error: calling addArc() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    List adj = G->adjLists[u];
    moveFront(adj);
    while (index(adj) >= 0 && get(adj) < v){
        //printf("index: %d\n", index(adj));
        moveNext(adj);
    }
    if (index(adj) == -1){
        // printf("index (in 1st case): %d\n", index(adj));
        append(adj, v);
    }
    else{
        // printf("index (in 2nd case): %d\n", index(adj));
        if (get(adj) != v){
            insertBefore(adj, v);
        }
        else{
            G->size -= 1;
        }
    }
    G->size += 1;
}

// addEdge()
// inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢.
// Pre: 1<=u<=n, 1<=v<=n
void addEdge(Graph G, int u, int v){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling addEdge() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling addEdge() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if (v < 1 || v > getOrder(G)){
        fprintf(stderr, "Graph Error: calling addEdge() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    addArc(G, u, v);
    addArc(G, v, u);
    G->size -= 1;
}

// Visit()
// Private helper function only called by DFS()
int Visit(Graph G, List S, int x, int time){
    G->discover[x] = (++time);
    G->color[x] = 1;
    List adj = G->adjLists[x];
    for (moveFront(adj); index(adj) >= 0; moveNext(adj)){
        if (G->color[get(adj)] == 0){
            G->parent[get(adj)] = x;
            time = Visit(G, S, get(adj), time);
        }
    }
    G->color[x] = 2;
    G->finish[x] = (++time);
    prepend(S, x);
    return time;
}

// DFS()
// performs the depth first search algorithm on 𝐺. 
// The List argument 𝑆 defines the order in which vertices are to be processed in the main loop of DFS. 
// Second, when DFS is complete, it will store the vertices by decreasing finish times. 
// Pre: length(S)==getOrder(G)
void DFS(Graph G, List S){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling DFS() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(S) != getOrder(G)){
        fprintf(stderr, "Graph Error: calling DFS() with incomplete stack\n");
        exit(EXIT_FAILURE);
    }
    for (int x = 1; x <= getOrder(G); x++){
        G->color[x] = 0;
        G->parent[x] = NIL;
    }
    int time = 0;
    List cS = copyList(S);
    clear(S);
    for (moveFront(cS); index(cS) >= 0; moveNext(cS)){
        if (G->color[get(cS)] == 0){
            time = Visit(G, S, get(cS), time);
        }
    }
    freeList(&cS);
}

// Other Functions

// copyGraph()
// returns a reference to a new graph that is a copy of G
Graph copyGraph(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling copyGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    Graph out = newGraph(getOrder(G));
    for (int i = 1; i <= getOrder(G); i++){
        List adj = G->adjLists[i];
        for (moveFront(adj); index(adj) >= 0; moveNext(adj)){
            append(out->adjLists[i], get(adj));
        }
    }
    return out;
}

// transpose()
// returns a reference to a new graph object representing the transpose of G
Graph transpose(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling copyGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    Graph out = newGraph(getOrder(G));
    for (int i = 1; i <= getOrder(G); i++){
        List adj = G->adjLists[i];
        for (moveFront(adj); index(adj) >= 0; moveNext(adj)){
            append(out->adjLists[get(adj)], i);
        }
    }
    return out;
}

// printGraph()
// prints the adjacency list representation of 𝐺 to the file pointed to by out.
void printGraph(FILE* out, Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling printGraph() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (out==NULL){
        fprintf(stderr, "Graph Error: calling printGraph() on NULL File pointer\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= G->order; i++){
        List adj = G->adjLists[i];
        fprintf(out, "%d: ", i);
        for (moveFront(adj); index(adj) >= 0; moveNext(adj)){
            fprintf(out, "%d ", get(adj));
        }
        fprintf(out, "\n");
    }
}