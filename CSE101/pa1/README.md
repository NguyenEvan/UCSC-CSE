
# PA1 
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA1 
* README.md
* Table of contents for List ADT 

## List.h 
Contains the function declarations n eeded for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. The "List" datatype defined for the Client is a pointer to a struct containing its actual implementation. 
## List.c
Contains the function definitions that implement the declarations found in List.h. Also implements the NodeObj struct and aforementioned ListObj struct, which cannot be directly accessed by the client.
## ListTest.c
Contains brief test cases for each function in List.h. The first part of the program is a copy of the provided file ListClient.c. The expected output can be seen in a comment at the end of the file.   
## Lex.c
Receives an input and output file as command line arguments. Uses the List ADT implementation to lexicographically sort the input strings by sorting indices (i.e. from 0 to n-1, where n is the number of strings in the input file). Once the indices are sorted, they are used to write the lexicographically-sorted strings to the output file.   
## Makefile
Slightly modified to include functionality for creating a ListTest executable and checking for its memory leaks