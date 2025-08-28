
# PA2
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA2
* README.md
* Table of contents for Graph ADT 

## List.h 
Contains the function declarations n eeded for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. The "List" datatype defined for the Client is a pointer to a struct containing its actual implementation. 
## List.c
Contains the function definitions that implement the declarations found in List.h. Also implements the NodeObj struct and aforementioned ListObj struct, which cannot be directly accessed by the client.
## GraphTest.c
Contains brief test cases for each function in Graph.h.
## FindPath.c
Receives an input and output file as command line arguments. Uses the Graph ADT implementation to print an adjacency list representation of a graph, and identify shortest paths and distances between two vertices. 
## Makefile
Slightly modified to include functionality for creating a GraphTest executable
## Graph.h 
Contains the function declarations neded for the client to effectively create, access, manipulate, and free the fields and memory associated with the Graph ADT. The "Graph" datatype defined for the Client is a pointer to a struct containing its actual implementation.
## Graph.c
Contains the function definitions that implement the declarations found in Graph.h. 