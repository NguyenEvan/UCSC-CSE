# PA5
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA5
* README.md
* Table of contents for List ADT 

## List.h 
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. Nothing is changed from the given file provided by the professor.
## List.cpp
Contains the function definitions that implement the declarations found in List.h. Also contains an inner Node struct which is part of the List ADT's private fields. Contains implementations for member functions, overloaded operators.
## ListTest.cpp
Contains brief test cases for each function in List.h.  
## Shuffle.cpp
Receives a number from the command line as input. This number represents the number of simulations that will occur. Simulation #n will perform a perfect shuffle a deck of cards from 0 to n until it is back to the original. The number of shuffles it takes is returned to the output stream. All these simulation results will be displayed in an output table. A function called shuffle() is defined in the client file above main() that performs a perfect shuffle in-place on the List ADT.
## Makefile
Slightly modified to include functionality for creating a ListTest executable and checking for its memory leaks