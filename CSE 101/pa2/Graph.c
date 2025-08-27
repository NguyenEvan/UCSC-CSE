/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA2
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
   int* distance; // array of ints whose ith element is the distance from the most recent source to vertex i
   int order; // number of vertices
   int size; // number of edges
   int lastSource;
} GraphObj;

/*** Constructors-Destructors ***/ 

// newGraph()
// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges.
Graph newGraph(int n){
    Graph G = malloc(sizeof(GraphObj));
    G->order = n;
    G->size = 0;
    G->adjLists = malloc(sizeof(List) * (n+1));
    G->color = malloc(sizeof(int) * (n+1));
    G->parent = malloc(sizeof(int) * (n+1));
    G->distance = malloc(sizeof(int) * (n+1));
    G->adjLists[0] = newList();
    for (int i = 1; i < n + 1; i++){
        G->parent[i] = G->color[i] = NIL;
        G->distance[i] = INF;
        G->adjLists[i] = newList();
    }
    G->lastSource = NIL;
    return G;

} 

// freeGraph()
// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL.
void freeGraph(Graph* pG){

    if( pG!=NULL && *pG!=NULL ){
        free((*pG)->color);
        free((*pG)->parent);
        free((*pG)->distance);
        for (int i = 0; i <= (*pG)->order; i++){
            freeList(&((*pG)->adjLists[i]));
        }
        free((*pG)->adjLists);
        free(*pG);
        *pG = NULL;
   }
}

/*** Access functions ***/ 

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

// getSource()
// returns the source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called.
int getSource(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getSource() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    return G->lastSource;
}

// getParent()
// return the parent of vertex 𝑢 in the BFS tree created by BFS(), or NIL if BFS() has not yet been called.
// pre: 1≤𝑢≤getOrder(𝐺)
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

// getDist()
// returns the distance from the most recent BFS source to vertex 𝑢, or INF if BFS() has not yet been called.
// pre: 1≤𝑢≤getOrder(𝐺)
int getDist(Graph G, int u){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getDist() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling getDist() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    return G->distance[u];
}

// getPath()
// appends to the List 𝐿 the vertices of a shortest path in 𝐺 from source to 𝑢, or appends to 𝐿 the value NIL if no such path exists.
// pre: getSource(G)!=NIL, 1≤𝑢≤getOrder(𝐺)
void getPath(List L, Graph G, int u){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling getPath() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > getOrder(G)){
        fprintf(stderr, "Graph Error: calling getPath() on an invalid vertex\n");
        exit(EXIT_FAILURE);
    }
    if (getSource(G) == NIL){
        fprintf(stderr, "Graph Error: calling getPath() before calling BFS(\n");
        exit(EXIT_FAILURE);
    }

    int source = G->lastSource;
    if (source == u){
        append(L, source);
    }
    else if (G->parent[u] == NIL){
        clear(L);
        append(L, NIL);
    }
    else{
        getPath(L, G, G->parent[u]);
        append(L, u);
    }
} 

/*** Manipulation procedures ***/ 

// makeNull()
// deletes all edges of 𝐺, restoring it to its original (no edge) state.
void makeNull(Graph G){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling makeNull() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    G->size = 0;
    for (int i = 1; i <= G->order; i++){
        clear(G->adjLists[i]);
    }
} 

// addEdge()
// inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢.
// pre: 1≤𝑢≤getOrder(𝐺), 1≤v≤getOrder(𝐺)
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

// addArc()
// inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣)
// pre: 1≤𝑢≤getOrder(𝐺), 1≤v≤getOrder(𝐺)
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
        insertBefore(adj, v);
    }
    G->size += 1;
} 

// BFS()
// runs the BFS algorithm on the Graph 𝐺 with source 𝑠, setting the color, distance, parent, and source fields of 𝐺 accordingly
// pre: 1≤s≤getOrder(𝐺)
void BFS(Graph G, int s){
    if (G==NULL){
        fprintf(stderr, "Graph Error: calling BFS() on NULL Graph reference\n");
        exit(EXIT_FAILURE);
    }
    if (s < 1 || s > getOrder(G)){
        fprintf(stderr, "Graph Error: calling BFS() on an invalid source vertex\n");
        exit(EXIT_FAILURE);
    }
    G->lastSource = s;
    for (int i = 1; i < G->order + 1; i++){
        if (i == s){
            G->color[s] = 1;
            G->distance[s] = 0;
            G->parent[s] = NIL;
        }
        else{
            G->color[i] = 0;
            G->distance[i] = INF;
            G->parent[i] = NIL;
        }
    }
    List Q = newList();
    prepend(Q, s);
    while (length(Q) > 0){
        int x = back(Q);
        deleteBack(Q);
        List currAdj = G->adjLists[x];
        for(moveFront(currAdj); index(currAdj)>=0; moveNext(currAdj)){
            int y = get(currAdj);
            if (G->color[y] == 0){
                G->color[y] = 1;
                G->distance[y] = G->distance[x] + 1;
                G->parent[y] = x;
                prepend(Q, y);
            }
        }
        G->color[x] = 2;
    }
    freeList(&Q);
}

/*** Other operations ***/ 

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
