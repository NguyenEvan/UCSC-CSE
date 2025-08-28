# PA6
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA6
* README.md
* Table of contents for BigInteger ADT 

## List.h 
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the List ADT. Nothing is changed from the given file provided by the professor.
## List.cpp
Contains the function definitions that implement the declarations found in List.h. Also contains an inner Node struct which is part of the List ADT's private fields. Contains implementations for member functions, overloaded operators.
## ListTest.cpp
Contains brief test cases for each function in List.h.  
## BigInteger.h
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the BigInteger ADT. Nothing is changed from the given file provided by the professor.
## BigInteger.cpp
Contains the function definitions that implement the declarations found in BigInteger.h. Contains numerous non-member helper functions that focus on a seamless implementation of the required overloaded operators (*, +, -), given that they work as intended.
## BigIntegerTest.cpp
Contains brief test cases for each (non-helper) function in BigInteger.c.
## Arithmetic.cpp
Reads two string representations of a BigInteger from a given input file and performs numerous operations on them, printing them to a given output file.
## Makefile
Slightly modified to include functionality for creating a ListTest and BigIntegerTest executable.