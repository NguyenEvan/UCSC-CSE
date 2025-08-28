/***
* Cache.c
* C file for Cache ADT 
***/ 

//-----------------------------------------------------------------------------
// Cache.c
// Implementation file for Cache ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <string.h>
#include<stdbool.h>
#include "Cache.h"


// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct CacheNodeObj{
   char* response;
   int responseLen;
   char* host;
   int port;
   char* uri;
   CacheNode next;
   CacheNode prev;
} CacheNodeObj;

// private CacheObj type
typedef struct CacheObj{
   CacheNode front;
   CacheNode back;
   int length;
   int n;
   char* mode;
} CacheObj;


// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
CacheNode newNode(char* response, int responseLen, char* host, int port, char* uri){
   CacheNode N = malloc(sizeof(CacheNodeObj));
   assert( N!=NULL );
   N->response = response;
   N->responseLen = responseLen;
   N->host = host;
   N->port = port;
   N->uri = uri;
   N->next = NULL;
   N->prev = NULL;
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(CacheNode* pN){
   if( pN!=NULL && *pN!=NULL ){
      free((*pN)->response);
      free((*pN)->uri);
      free((*pN)->host);
      free(*pN);
      *pN = NULL;
   }
}

// newCache()
// Returns reference to new empty Queue object.
Cache newCache(char* mode, int n){
   Cache C;
   C = malloc(sizeof(CacheObj));
   assert( C!=NULL );
   C->front = C->back = NULL;
   C->length = 0;
   C->mode = mode;
   C->n = n;
   return(C);
}

// freeCache()
// Frees all heap memory associated with Queue *pQ, and sets *pQ to NULL.
void freeCache(Cache* pC){
   if(pC!=NULL && *pC!=NULL) { 
      while(length(*pC) > 0) {
        evictCache(*pC); 
      }
      free(*pC);
      *pC = NULL;
   }
}

// Access functions -------------------------------------------

int length(Cache C){
    return C->length;
}

char* getResponse(CacheNode N){
    return N->response;
}

int getResponseLen(CacheNode N){
    return N->responseLen;
}

int getCacheSize(Cache C){
    return C->n;
}

// Modified searchCache function - removed header addition
CacheNode searchCache(Cache C, char* host, int port, char* uri){
    CacheNode N = C->front;
    while (N != NULL){
        if (strcmp(N->host, host) == 0 && strcmp(N->uri, uri) == 0 && N->port == port){
            if (strcmp(C->mode, "LRU") == 0){
                // If N is already at the front or it's the only node, no need to move it
                if (N == C->front || length(C) == 1) {
                    // No change needed
                } 
                else {
                    // Remove N from its current position
                    if (N->prev) N->prev->next = N->next;
                    if (N->next) N->next->prev = N->prev;
                    
                    // Update back pointer if N was the back node
                    if (N == C->back) {
                        C->back = N->prev;
                        C->back->next = NULL;
                    }
                    
                    // Move N to the front
                    N->next = C->front;
                    N->prev = NULL;
                    C->front->prev = N;
                    C->front = N;
                }
            }
            return N;
        }
        N = N->next;
    }
    return NULL;
}

// Operation functions ------------------------------------------------------

// Modified pushCache function - add "Cached: True" header when storing response
void pushCache(Cache C, char* response, int responseLen, char* host, int port, char* uri){
    // Add "Cached: True" header to the response before storing
    char cacheHeader[] = "Cached: True";
    char* modifiedResponse = NULL;
    int modifiedResponseLen = responseLen;
    
    // Find the end of headers (double CRLF)
    char* headersEnd = strstr(response, "\r\n\r\n");
    if (headersEnd) {
        // Calculate lengths properly
        int headersLen = headersEnd - response;
        int bodyLen = responseLen - headersLen; // This includes the \r\n\r\n
        
        // Allocate space for modified response
        modifiedResponse = malloc(responseLen + strlen(cacheHeader) + 2);
        if (modifiedResponse != NULL) {
            // Copy up to end of headers
            memcpy(modifiedResponse, response, headersLen);
            
            // Add Cached header
            memcpy(modifiedResponse + headersLen, "\r\n", 2);
            memcpy(modifiedResponse + headersLen + 2, cacheHeader, strlen(cacheHeader));
            
            // Copy rest of response including second CRLF
            memcpy(modifiedResponse + headersLen + 2 + strlen(cacheHeader), headersEnd, bodyLen);
            
            // Update response length
            modifiedResponseLen = headersLen + 2 + strlen(cacheHeader) + bodyLen;
        } else {
            // If malloc fails, fall back to original response
            modifiedResponse = response;
        }
    } else {
        // If no headers end found, use original response
        modifiedResponse = response;
    }
    
    // Create new cache entry with modified response
    CacheNode newEntry;
    if (modifiedResponse != response) {
        // If we created a new response buffer, use it
        newEntry = newNode(modifiedResponse, modifiedResponseLen, host, port, uri);
        free(response); // Free the original response
    } else {
        // Otherwise use original response
        free(modifiedResponse);
        newEntry = newNode(response, responseLen, host, port, uri);
    }
    
    // Insert at front of cache
    if (length(C) == 0){
        C->front = C->back = newEntry;
    }
    else{
        newEntry->next = C->front;
        C->front->prev = newEntry;
        C->front = newEntry;
    }
    C->length += 1;

    // Evict if cache is full
    if (C->n != 0 && C->length > C->n){
        evictCache(C);
    }
    return;
}

void evictCache(Cache C){
    CacheNode oldBack = C->back;
    if (length(C) == 1){
        freeNode(&oldBack);
        C->front = C->back = NULL;
        C->length = 0;
    }
    else{
        CacheNode newBack = C->back->prev;
        newBack->next = NULL;
        oldBack->next = oldBack->prev = NULL;
        freeNode(&oldBack);
        C->back = newBack;
        C->length -= 1;

    }
}


void printCache(FILE* out, Cache L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (out==NULL){
        fprintf(stderr, "List Error: calling printList() on NULL File pointer\n");
        exit(EXIT_FAILURE);
    }
    CacheNode currNode = L->front;
    while (currNode){
        // printf("curNode->data: %d\n", currNode->data);
        fprintf(out, "%s", currNode->response);
        currNode = currNode->next;
        if (currNode != NULL) {
            fprintf(out, " ");
        }
    }
}
