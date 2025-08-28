# PA7
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA7
* README.md
* Table of contents for Dictionary ADT 

## Dictionary.h 
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the Dictionary ADT. None of the provided helper member functions have been tweaked. However, I added one named "Transplant" that helped me to implement the remove() function. It replaces the given subtree rooted at u with the subtree rooted at v.
## Dictionary.cpp
Contains the function definitions that implement the declarations found in Dictionary.h. Also contains an inner Node struct and constructor which is part of the Dictionary ADT's private fields. Contains implementations for member and helper functions as well as overloaded operators.
## DictionaryTest.cpp
Contains brief test cases for each function in Dictionary.h.  
## Order.cpp
Reads strings from an input file, which are used as keys in a dictionary. Its corresponding values are the line number in which they were read. The client then outputs an in-order representation of the dictionary that contains its key-value pairs as well as a pre-order representation that only contains its keys.
## Makefile
Slightly modified to include functionality for creating a DictionaryTest executable.