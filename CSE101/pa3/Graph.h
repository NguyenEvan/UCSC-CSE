/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA3
* Graph.h 
* Header file for Graph ADT 
***/ 
#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "List.h"
#define UNDEF -1 // undefined time
#define NIL 0 // undefined vertex label
typedef struct GraphObj* Graph;

// Constructors-Destructors

// newGraph()
// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges.
Graph newGraph(int n);

// freeGraph()
// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL.
void freeGraph(Graph* pG);

// Access functions

// getOrder()
// returns the order of the graph (number of vertices)
int getOrder(Graph G);

// getSize()
// returns the size of the graph (number of edges)
int getSize(Graph G);

// getParent()
// return the parent of vertex u in the DFS forest created by DFS(), or NIL if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G) 
int getParent(Graph G, int u);

// getDiscover()
// returns the discover time of vertex u in the DFS forest created by DFS(), or UNDEF if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G) 
int getDiscover(Graph G, int u); 

// getFinish()
// returns the finish time of vertex u in the DFS forest created by DFS(), or UNDEF if DFS() has not yet been called.
// Pre: 1<=u<=n=getOrder(G) 
int getFinish(Graph G, int u);

// Manipulation procedures

// addArc()
// inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣)
// Pre: 1<=u<=n, 1<=v<=n 
void addArc(Graph G, int u, int v); 

// addEdge()
// inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢.
// Pre: 1<=u<=n, 1<=v<=n 
void addEdge(Graph G, int u, int v);

// DFS()
// performs the depth first search algorithm on 𝐺. 
// The List argument 𝑆 defines the order in which vertices are to be processed in the main loop of DFS. 
// Second, when DFS is complete, it will store the vertices by decreasing finish times. 
// Pre: length(S)==getOrder(G) 
void DFS(Graph G, List S);

// Other Functions

// transpose()
// returns a reference to a new graph object representing the transpose of G
Graph transpose(Graph G);

// copyGraph()
// returns a reference to a new graph that is a copy of G
Graph copyGraph(Graph G);

// printGraph()
// prints the adjacency list representation of 𝐺 to the file pointed to by out.
void printGraph(FILE* out , Graph G);

#endif