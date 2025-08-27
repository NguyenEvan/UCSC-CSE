
# PA3
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA3
* README.md
* Table of contents for Graph ADT 

## List.h 
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. The "List" datatype defined for the Client is a pointer to a struct containing its actual implementation. 
## List.c
Contains the function definitions that implement the declarations found in List.h. Also implements the NodeObj struct and aforementioned ListObj struct, which cannot be directly accessed by the client.
## GraphTest.c
Contains brief test cases for each function in Graph.h.
## FindComponents.c
Receives an input and output file as command line arguments. Uses the Graph ADT implementation to print an adjacency list representation of a graph, and identify its strongly connected components by calling DFS() twice on the original graph and its transpose
## Makefile
Slightly modified to include functionality for creating a GraphTest executable
## Graph.h 
Contains the function declarations neded for the client to effectively create, access, manipulate, and free the fields and memory associated with the Graph ADT. The "Graph" datatype defined for the Client is a pointer to a struct containing its actual implementation.
## Graph.c
Contains the function definitions that implement the declarations found in Graph.h. DFS() uses an internal helper function called Visit(), which is called recursively. Concerning the "time" variable, I passed Visit() its own local copy and had the function return the updated variable, so both DFS() and Visit() can have an equivalent representation of the variable at all times. This file uses many of the implementations from PA2. Functions like printGraph() and getOrder() are identical while functions like newGraph() and freeGraph() are slightly tweaked to add the functionality for discovery and finish time. Obviously, any functions associated with DFS() like transpose() are new.