# PA8
## README.md
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA8
* README.md
* Table of contents for Dictionary ADT 

## Dictionary.h 
Contains the function declarations needed for the client to effectively create, access, manipulate, and free the fields and memory associated with the Dictionary ADT. None of the provided helper member functions have been tweaked nor have any member functions been added.
## Dictionary.cpp
Contains the function definitions that implement the declarations found in Dictionary.h. Many functions are borrowed from PA7. Also contains an inner Node struct and constructor which is part of the Dictionary ADT's private fields. Contains implementations for member and helper functions as well as overloaded operators. Helper functions do a great majority of the work for the ADT operations.
## DictionaryTest.cpp
Contains brief test cases for each function in Dictionary.h. Mostly borrowed from PA7.  
## Order.cpp
Reads strings from an input file, which are used as keys in a dictionary. Its corresponding values are the line number in which they were read. The client then outputs an in-order representation of the dictionary that contains its key-value pairs as well as a pre-order representation that only contains its keys. Completely borrowed from PA7.
## WordFrequency.cpp
Reads strings from an input file, which are processed to remove any special characters apart from words that are then converted to lowercase. The client then outputs a dictionary's key-value pairs to the output file, where the key represents a unique word in the input file and the value represents its frequency. All credit for code used for C++ tokenization goes to the professor.
## Makefile
Slightly modified to include functionality for creating a DictionaryTest and Order executable.