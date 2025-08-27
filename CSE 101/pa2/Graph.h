/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA2 
* Graph.h 
* Header file for Graph ADT 
***/ 
#ifndef GRAPH_H_INCLUDE_
#define GRAPH_H_INCLUDE_
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "List.h"
#define INF -1 // infinity (not reachable)
#define NIL -2 // undefined vertex label
typedef struct GraphObj* Graph;

/*** Constructors-Destructors ***/

// newGraph()
// returns a Graph pointing to a newly created GraphObj representing a graph having n vertices and no edges.
Graph newGraph(int n); 

// freeGraph()
// frees all heap memory associated with the Graph *pG, then sets the handle *pG to NULL.
void freeGraph(Graph* pG); 

/*** Access functions ***/ 

// getOrder()
// returns the order of the graph (number of vertices)
int getOrder(Graph G); 

// getSize()
// returns the size of the graph (number of edges)
int getSize(Graph G); 

// getSource()
// returns the source vertex most recently used in function BFS(), or NIL if BFS() has not yet been called.
int getSource(Graph G); 

// getParent()
// return the parent of vertex 𝑢 in the BFS tree created by BFS(), or NIL if BFS() has not yet been called.
// pre: 1≤𝑢≤getOrder(𝐺)
int getParent(Graph G, int u); 

// getDist()
// returns the distance from the most recent BFS source to vertex 𝑢, or INF if BFS() has not yet been called.
// pre: 1≤𝑢≤getOrder(𝐺)
int getDist(Graph G, int u); 

// getPath()
// appends to the List 𝐿 the vertices of a shortest path in 𝐺 from source to 𝑢, or appends to 𝐿 the value NIL if no such path exists.
// pre: getSource(G)!=NIL, 1≤𝑢≤getOrder(𝐺)
void getPath(List L, Graph G, int u); 

/*** Manipulation procedures ***/ 

// makeNull()
// deletes all edges of 𝐺, restoring it to its original (no edge) state.
void makeNull(Graph G); 

// addEdge()
// inserts a new edge joining 𝑢 to 𝑣, i.e. 𝑢 is added to the adjacency List of 𝑣, and 𝑣 to the adjacency List of 𝑢.
// pre: 1≤𝑢≤getOrder(𝐺).
void addEdge(Graph G, int u, int v); 

// addArc()
// inserts a new directed edge from 𝑢 to 𝑣, i.e. 𝑣 is added to the adjacency List of 𝑢 (but not 𝑢 to the adjacency List of 𝑣)
// pre: 1≤𝑢≤getOrder(𝐺).
void addArc(Graph G, int u, int v);

// BFS()
// runs the BFS algorithm on the Graph 𝐺 with source 𝑠, setting the color, distance, parent, and source fields of 𝐺 accordingly
// pre: 1≤s≤getOrder(𝐺)
void BFS(Graph G, int s); 

/*** Other operations ***/ 

// printGraph()
// prints the adjacency list representation of 𝐺 to the file pointed to by out.
void printGraph(FILE* out, Graph G); 

#endif