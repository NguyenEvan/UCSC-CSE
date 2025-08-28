/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4
* List.c
* C file for List ADT 
***/ 

//-----------------------------------------------------------------------------
// List.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <string.h>
#include<stdbool.h>
#include "List.h"


// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj* Node;

// private NodeObj type
typedef struct NodeObj{
   void* data;
   Node next;
   Node before;
} NodeObj;

// private ListObj type
typedef struct ListObj{
   Node front;
   Node back;
   Node cursor;
   int length;
   int cursorIndex;
} ListObj;


// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next and data fields.
Node newNode(void* data){
   Node N = malloc(sizeof(NodeObj));
   assert( N!=NULL );
   N->data = data;
   N->next = NULL;
   N->before = NULL;
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

// newList()
// Returns reference to new empty Queue object.
List newList(){
   List L;
   L = malloc(sizeof(ListObj));
   assert( L!=NULL );
   L->front = L->back = L->cursor = NULL;
   L->length = 0;
   L->cursorIndex = -1;
   return(L);
}

// freeList()
// Frees all heap memory associated with Queue *pQ, and sets *pQ to NULL.
void freeList(List* pL){
   if(pL!=NULL && *pL!=NULL) { 
      while(length(*pL) > 0) {
        deleteFront(*pL); 
      }
      free(*pL);
      *pL = NULL;
   }
}


// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L){
    if (L==NULL){
        return 0;
    }
    return L->length;
}

// index()
// Returns index of cursor element if defined, -1 otherwise.
int index(List L){
    if (L==NULL || L->cursor==NULL){
        return -1;
    }
    return L->cursorIndex;
}

// front()
// Returns front element of L. 
// Pre: length()>0
void* front(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->front->data;

}

// back()
// Returns back element of L. 
// Pre: length()>0
void* back(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    return L->back->data;
}

// get()
// Returns cursor element of L. 
// Pre: length()>0, index()>=0 
void* get(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0){
        fprintf(stderr, "List Error: calling get() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    return L->cursor->data;
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets L to its original empty state. 
void clear(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling clear() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    while (length(L) > 0){
        deleteFront(L);
    }
    L->cursor = NULL;
    L->cursorIndex = -1;
    L->length = 0;
}

// set()
// Overwrites the cursor element’s data with x. 
// Pre: length()>0, index()>=0 
void set(List L, void* x){
    if (L==NULL){
        fprintf(stderr, "List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0){
        fprintf(stderr, "List Error: calling set() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    L->cursor->data = x;
}

// moveFront()
// If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) > 0){
        L->cursor = L->front;
        L->cursorIndex = 0;
    }
}

// moveBack()
// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) > 0){
        L->cursor = L->back;
        L->cursorIndex = length(L) - 1;
    }
}

// movePrev()
// If cursor is defined and not at front, move cursor one  
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined do nothing
void movePrev(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if (L->cursor != NULL){
        L->cursor = L->cursor->before;
        if (L->cursor == NULL){
            L->cursorIndex = -1;
        }
        else{
            L->cursorIndex -= 1;
        }
    }
}

// moveNext()
// If cursor is defined and not at back, move cursor one  
// step toward the back of L; if cursor is defined and at  
// back, cursor becomes undefined; if cursor is undefined do nothing
void moveNext(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor != NULL){
        L->cursor = L->cursor->next;
        if (L->cursor == NULL){
            L->cursorIndex = -1;
        }
        else{
            L->cursorIndex += 1;
        }
    }
    
}

// prepend()
// Insert new element into L. If L is non-empty, insertion takes place before front element.
void prepend(List L, void* x){
    if (L==NULL){
        fprintf(stderr, "List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if (length(L) == 0){
        L->front = L->back = newNode(x);
    }
    else{
        Node prependNode = newNode(x);
        prependNode->next = L->front;
        L->front->before = prependNode;
        L->front = prependNode;
        if (L->cursor != NULL){
            L->cursorIndex += 1;
        }
    }

    L->length += 1;
}

// append()
// Insert new element into L. If L is non-empty, insertion takes place after back element.
void append(List L, void* x){
    if (L==NULL){
        fprintf(stderr, "List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    
    if (length(L) == 0){
        L->front = L->back = newNode(x);
    }
    else{
        Node appendNode = newNode(x);
        appendNode->before = L->back;
        L->back->next = appendNode;
        L->back = appendNode;
    }
    L->length += 1;
}

// insertBefore()
// Insert new element before cursor. 
// Pre: length()>0, index()>=0
void insertBefore(List L, void* x){
    if (L==NULL){
        fprintf(stderr, "List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0){
        fprintf(stderr, "List Error: calling insertBefore() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node insertNode = newNode(x);
    if (L->cursor == L->front){
        insertNode->next = L->cursor;
        L->cursor->before = insertNode;
        L->front = insertNode;
        L->length += 1;
        L->cursorIndex += 1;
    }
    else{
        insertNode->next = L->cursor;
        insertNode->before = L->cursor->before;
        L->cursor->before->next = insertNode;
        L->cursor->before = insertNode;
        L->length += 1;
        L->cursorIndex += 1;
    }
}

// insertAfter()
// Insert new element after cursor.  
// Pre: length()>0, index()>=0
void insertAfter(List L, void* x){
    if (L==NULL){
        fprintf(stderr, "List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling insertAfter() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0){
        fprintf(stderr, "List Error: calling insertAfter() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    Node insertNode = newNode(x);
    if (L->cursor == L->back){
        insertNode->before = L->cursor;
        L->cursor->next = insertNode;
        L->back = insertNode;
        L->length += 1;
    }
    else{
        insertNode->before = L->cursor;
        insertNode->next = L->cursor->next;
        L->cursor->next->before = insertNode;
        L->cursor->next = insertNode;
        L->length += 1;
    }
}

// deleteFront()
// Delete the front element. 
// Pre: length()>0 
void deleteFront(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 1){
        freeNode(&L->front);
        L->back = L->cursor = NULL;
        L->length = 0;
        L->cursorIndex = -1;
    }
    else if (L->front == L->cursor){
        Node newFront = L->front->next;
        newFront->before = NULL;
        freeNode(&L->front);
        L->front = newFront;
        L->length -= 1;
        L->cursorIndex = -1;
        L->cursor = NULL;
    }
    else{
        Node newFront = L->front->next;
        newFront->before = NULL;
        freeNode(&L->front);
        L->front = newFront;
        L->cursorIndex -= 1;
        L->length -= 1;
    }
}

// deleteBack()
// Delete the back element. 
// Pre: length()>0 
void deleteBack(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 1){
        freeNode(&L->back);
        L->front = L->cursor = NULL;
        L->length = 0;
        L->cursorIndex = -1;
    }
    else if (L->back == L->cursor){
        Node newBack = L->back->before;
        newBack->next = NULL;
        freeNode(&L->back);
        L->back = newBack;
        L->length -= 1;
        L->cursorIndex = -1;
        L->cursor = NULL;
    }
    else{
        Node newBack = L->back->before;
        newBack->next = NULL;
        freeNode(&L->back);
        L->back = newBack;
        L->length -= 1;
    }
}

// delete()
// Delete cursor element, making cursor undefined. 
// Pre: length()>0, index()>=0
void delete(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L) == 0){
        fprintf(stderr, "List Error: calling delete() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if (index(L) < 0){
        fprintf(stderr, "List Error: calling delete() with an undefined cursor\n");
        exit(EXIT_FAILURE);
    }
    if (L->cursor == L->front){
        deleteFront(L);
        L->cursorIndex = -1;
    }
    else if (L->cursor == L->back){
        deleteBack(L);
        L->cursorIndex = -1;
    }
    else{
        Node prev = L->cursor->before;
        Node after = L->cursor->next;
        freeNode(&L->cursor);
        prev->next = after;
        after->before = prev;
        L->length -= 1;
        L->cursorIndex = -1;
    }
}

// Other operations -----------------------------------------------------------

// printList()
// Prints to the file pointed to by out, a  
// string representation of L consisting  
// of a space separated sequence of integers, 
// with front on left.
void printList(FILE* out, List L){
    if (L==NULL){
        fprintf(stderr, "List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (out==NULL){
        fprintf(stderr, "List Error: calling printList() on NULL File pointer\n");
        exit(EXIT_FAILURE);
    }
    Node currNode = L->front;
    for (int i = 0; i < length(L); i++){
        // printf("curNode->data: %d\n", currNode->data);
        fprintf(out, "%p", currNode->data);
        currNode = currNode->next;
        if (currNode != NULL) {
            fprintf(out, " ");
        }
    }
}
