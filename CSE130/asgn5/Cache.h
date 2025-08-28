/***
* Cache.h 
* Header file for Cache ADT 
***/ 

#ifndef Cache_H_INCLUDE_
#define Cache_H_INCLUDE_
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
typedef struct CacheObj* Cache;
typedef struct CacheNodeObj* CacheNode;

// Constructors-Destructors --------------------------------------------------- 

// newCache()
// Creates and returns a new empty Cache.
Cache newCache(char* mode, int n); 

// freeCache()
// Frees all heap memory associated with *pL, and sets *pL to NULL.
void freeCache(Cache* pC);  

// Access functions

int length(Cache C);

char* getResponse(CacheNode N);

int getResponseLen(CacheNode N);

int getCacheSize(Cache C);

CacheNode searchCache(Cache C, char* host, int port, char* uri);

// Operation functions ----------------------------------------------------------

void pushCache(Cache C, char* response, int responseLen, char* host, int port, char* uri);

void evictCache(Cache C);    

// Other operations ----------------------------------------------------------- 

// printCache()
// Prints to the file pointed to by out, a  
// string representation of L consisting  
// of a space separated sequence of integers, 
// with front on left.
void printCache(FILE* out, Cache C);

#endif
