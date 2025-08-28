
# PA4
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4 
* README.md
* Table of contents for Matrix ADT 

## List.h 
Contains the function declarations n eeded for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. The "List" datatype defined for the Client is a pointer to a struct containing its actual implementation. The List ADT is slightly modified to hold void pointers instead of integers. 
## List.c
Contains the function definitions that implement the declarations found in List.h. Also implements the NodeObj struct and aforementioned ListObj struct, which cannot be directly accessed by the client. The List ADT is slightly modified to hold void pointers instead of integers. Implementations of List ADT functions will be unaware of what the void pointers point to. From this fact, I removed the implementations of copyList(), concatList(), and equals(), since these functions require the List ADT to be aware of the values that the pointers point to. If concatList()'s job was to simply connect two lists, then it would be possible to keep it. However, it returns a new list that holds a concatenation of two lists, and this requires that we create new versions of the List parameters, which makes it impossible to implement for similar reasons to copyList(). printList() is kept for visualization purposes in ListTest.c.
## ListTest.c
Contains brief test cases for each function in List.h. A slighly modified version of my ListTest.c from PA1, with its elements being a custom struct that I define in the file. 
## Matrix.h  
Contains the function declarations neded for the client to effectively create, access, manipulate, and free the fields and memory associated with the Matrix ADT. The "Matrix" datatype defined for the Client is a pointer to a struct containing its actual implementation.
## Matrix.c 
Contains the function definitions that implement the declarations found in Graph.h. Defines a private struct  referred to as Entry (actually a pointer to a struct called EntryObj). This holds the column and value data needed for the Matrix ADT and will be what the List ADT will use as its elements. 
## MatrixTest.c
Contains brief test cases for each function in Matrix.h.
## Sparse.c
Receives an input and output file as command line arguments. Uses the Matrix ADT implementations to take two equally-sized matrices from the input file and perform various matrix operations on one or both of them, including addition, scalar multiplication, subtraction, transpose, and multiplication
## Makefile
Slightly modified to include functionality for creating a ListClient and GraphClient executable. This was used in early testing stages.