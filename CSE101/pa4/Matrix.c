/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4
* List.c
* C file for Matrix ADT 
***/ 

//-----------------------------------------------------------------------------
// Matrix.c
// Implementation file for Matrix ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include <string.h>
#include<stdbool.h>
#include "List.h"
#include "Matrix.h"


// structs --------------------------------------------------------------------

// private Node type
typedef struct EntryObj* Entry;

// private NodeObj type
typedef struct EntryObj{
   int column;
   double data;
} EntryObj;

// private ListObj type
typedef struct MatrixObj{
   List* matrix;
   int size;
   int NNZ;
} MatrixObj;


// Constructors-Destructors ---------------------------------------------------

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    Matrix M = malloc(sizeof(MatrixObj));
    M->matrix = malloc(sizeof(List) * (n+1));
    for (int i = 0; i <= n; i++){
        M->matrix[i] = newList();
    }
    M->size = n;
    M->NNZ = 0;
    return M;
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    if(pM!=NULL && *pM!=NULL) {
        for (int i = 0; i <= (*pM)->size; i++){
            moveFront((*pM)->matrix[i]);
            while (index((*pM)->matrix[i]) >= 0){
                // printf("FREEING data: %.1f\n",((Entry) get((*pM)->matrix[i]))->data);
                // printf("FREEING column: %d\n",((Entry) get((*pM)->matrix[i]))->column);
                free(((Entry) get((*pM)->matrix[i])));
                moveNext((*pM)->matrix[i]);
            }
            freeList(&((*pM)->matrix[i]));
        } 
    }
    free((*pM)->matrix);
    free(*pM);
    *pM = NULL;
}

// Access functions
// size()
// Return the size of square Matrix M.
int size(Matrix M){
    if (M==NULL){
        return 0;
    }
    return M->size;
}

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    if (M==NULL){
        return 0;
    }
    return M->NNZ;
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
    if (A==NULL || B==NULL){
        fprintf(stderr, "Matrix Error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size || A->NNZ != B->NNZ){
        return false;
    }
    for (int i = 1; i <= A->size && i <= B->size; i++){
        List lA = A->matrix[i];
        List lB = B->matrix[i];
        if (length(lA) != length(lB)){
            return false;
        }
        for (moveFront(lA), moveFront(lB); index(lA) >= 0 && index(lB) >= 0; moveNext(lA), moveNext(lB)){
            if (((Entry)(get(lA)))->data != ((Entry)(get(lB)))->data || ((Entry)(get(lA)))->column != ((Entry)(get(lB)))->column){
                return false;
            }
        }
    }
    return true;
}

// Manipulation procedures
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    if (M==NULL){
        fprintf(stderr, "Matrix Error: calling makeZero() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= M->size; i++){
        for (moveFront(M->matrix[i]); index(M->matrix[i]) >= 0; moveNext(M->matrix[i])){
            free((Entry) get(M->matrix[i]));
        }
        clear(M->matrix[i]);
    }
    M->NNZ = 0;
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x){
    if (M==NULL){
        fprintf(stderr, "Matrix Error: calling changeEntry() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if ((i < 1 || i > M->size) || (j < 1 || j > M->size)){
        fprintf(stderr, "Matrix Error: calling changeEntry() with invalid row or column");
        exit(EXIT_FAILURE);
    }
    for (moveFront(M->matrix[i]); index(M->matrix[i]) >= 0; moveNext(M->matrix[i])){
        if (((Entry)get(M->matrix[i]))->column == j){
            if (x != 0){
                ((Entry)get(M->matrix[i]))->data = x;
            }
            else{
                M->NNZ -= 1;
                free((Entry) get(M->matrix[i]));
                delete(M->matrix[i]);
            }
            break;
            
        }
        if (((Entry)get(M->matrix[i]))->column > j){
            if (x != 0){
                M->NNZ += 1;
                Entry newEntry = malloc(sizeof(EntryObj));
                newEntry->column = j;
                newEntry->data = x;
                insertBefore(M->matrix[i], newEntry);
            }
            break;
        }
    }
    if (index(M->matrix[i]) == -1){
        if (x != 0){
            Entry newEntry = malloc(sizeof(EntryObj));
            newEntry->column = j;
            newEntry->data = x;
            append(M->matrix[i], newEntry);
            if (x != 0){
                M->NNZ += 1;
            }
        }
    }
    
}

// Matrix Arithmetic operations
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
    if (A==NULL){
        fprintf(stderr, "Matrix Error: calling copy() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Matrix out = newMatrix(A->size);
    out->NNZ = A->NNZ;
    for (int i = 1; i <= A->size; i++){
        for (moveFront(A->matrix[i]); index(A->matrix[i]) >= 0; moveNext(A->matrix[i])){
            Entry newEntry = malloc(sizeof(EntryObj));
            newEntry->data =((Entry)get(A->matrix[i]))->data;
            newEntry->column = ((Entry)get(A->matrix[i]))->column;
            append(out->matrix[i], newEntry);
        }
    }
    return out;
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A){
    if (A==NULL){
        fprintf(stderr, "Matrix Error: calling transpose() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Matrix out = newMatrix(A->size);
    out->NNZ = A->NNZ;
    for (int i = 1; i <= A->size; i++){
        if (length(A->matrix[i]) != 0){
            for (moveFront(A->matrix[i]); index(A->matrix[i]) >= 0; moveNext(A->matrix[i])){
                Entry newEntry = malloc(sizeof(EntryObj));
                newEntry->data =((Entry)get(A->matrix[i]))->data;
                newEntry->column = i;
                append(out->matrix[((Entry)get(A->matrix[i]))->column], newEntry);
            }
        }
    }
    return out;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A){
    if (A==NULL){
        fprintf(stderr, "Matrix Error: calling scalarMult() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    Matrix out = newMatrix(A->size);
    out->NNZ = A->NNZ;
    for (int i = 1; i <= A->size; i++){
        for (moveFront(A->matrix[i]); index(A->matrix[i]) >= 0; moveNext(A->matrix[i])){
            Entry newEntry = malloc(sizeof(EntryObj));
            newEntry->column = ((Entry)get(A->matrix[i]))->column;
            newEntry->data = ((Entry)get(A->matrix[i]))->data * x;
            append(out->matrix[i], newEntry);
        }
    }
    return out;
}

// aggRow()
// private helper function that calculates the aggregation of two lists and inputs it into an input-output parameter, which is also a list
// boolean variable sum notifies function whether it is performing addition or subtraction
void aggRow(List out, List A, List B, bool sum){
    for (moveFront(A); index(A) >= 0; moveNext(A)){
        Entry newEntry = malloc(sizeof(EntryObj));
        newEntry->column = ((Entry)get(A))->column;
        newEntry->data = ((Entry)get(A))->data;
        append(out, newEntry);
    }
    int mul = 1;
    if (!sum){
        mul = -1;
    }
    
    for (moveFront(B), moveFront(out); index(B) >= 0; moveNext(B)){
        while (index(out) >= 0 && ((Entry)get(out))->column < ((Entry)get(B))->column){
            moveNext(out);
        }
        
        if (index(out) != -1){
            // printf("COLUMN OF OUT: %d\n", ((Entry)get(out))->column);
            // printf("COLUMN OF B: %d\n", ((Entry)get(B))->column);
            if (((Entry)get(out))->column != ((Entry)get(B))->column){
                Entry newEntry = malloc(sizeof(EntryObj));
                newEntry->column = ((Entry)get(B))->column;
                newEntry->data = mul * ((Entry)get(B))->data;
                insertBefore(out, newEntry);
            }
            else{
                ((Entry)get(out))->data += mul * ((Entry)get(B))->data;
                if (((Entry)get(out))->data == 0){
                    free((Entry) get(out));
                    delete(out);
                    moveFront(out);
                }
            }
        }
        else{
            Entry newEntry = malloc(sizeof(EntryObj));
            newEntry->column = ((Entry)get(B))->column;
            newEntry->data = mul * ((Entry)get(B))->data;
            append(out, newEntry);
        }
        
    }

}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B){
    if (A==NULL || B==NULL){
        fprintf(stderr, "Matrix Error: calling sum() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size){
        fprintf(stderr, "Matrix Error: calling sum() on two matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix out = newMatrix(A->size);
    out->NNZ = 0;
    for (int i = 1; i <= A->size; i++){
        if (length(A->matrix[i]) != 0 || length(B->matrix[i]) != 0){
            aggRow(out->matrix[i], A->matrix[i], B->matrix[i], true);
            out->NNZ += length(out->matrix[i]);
        }
    }
    return out;
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B){
    if (A==NULL || B==NULL){
        fprintf(stderr, "Matrix Error: calling diff() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size){
        fprintf(stderr, "Matrix Error: calling diff() on two matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix out = newMatrix(A->size);
    out->NNZ = 0;
    for (int i = 1; i <= A->size; i++){
        if (length(A->matrix[i]) != 0 || length(B->matrix[i]) != 0){
            aggRow(out->matrix[i], A->matrix[i], B->matrix[i], false);
            out->NNZ += length(out->matrix[i]);
        }
    }
    return out;
}

// vectorDot()
// private helper function that calculates and returns the dot product of two lists
double vectorDot(List A, List B){
    double out = 0;
    if (length(A) == 0 || length(B) == 0){
        return 0;
    }
    for (moveFront(A), moveFront(B); index(A) >= 0 && index(B) >= 0; moveNext(A)){
        while (index(B) >= 0 && ((Entry)(get(B)))->column < ((Entry)(get(A)))->column){
            moveNext(B);
        }
        if (index(B) != -1 && ((Entry)(get(A)))->column == ((Entry)(get(B)))->column){
            out += ((Entry)(get(A)))->data * ((Entry)(get(B)))->data;
        }
    }
    return out;
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B){
    if (A==NULL || B==NULL){
        fprintf(stderr, "Matrix Error: calling product() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size){
        fprintf(stderr, "Matrix Error: calling product() on two matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix B_T = transpose(B);
    Matrix out = newMatrix(A->size);
    
    for (int i = 1; i <= A->size; i++){
        if (length(A->matrix[i]) != 0){
            for (int j = 1; j <= A->size; j++){
                if (length(B_T->matrix[j]) != 0){
                    double dot = vectorDot(A->matrix[i], B_T->matrix[j]);
                    if (dot != 0){
                        Entry newEntry = malloc(sizeof(EntryObj));
                        newEntry->column = j;
                        newEntry->data = dot;
                        append(out->matrix[i], newEntry);
                        out->NNZ += 1;
                    }
                }
            }
        }
    }
    freeMatrix(&B_T);
    
    return out;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.
void printMatrix(FILE* out, Matrix M){
    // printf("M->size: %d\n", M->size);
    for (int i = 1; i <= M->size; i++){
        if (length(M->matrix[i]) != 0){
            // printf("IN PRINTMATRIX\n");
            bool seen = false;
            bool printFlag = true;
            for (moveFront(M->matrix[i]); index(M->matrix[i]) >= 0; moveNext(M->matrix[i])){
                // printf("i: %d\n", i);
                if (printFlag){
                    fprintf(out, "%d: ", i);
                    printFlag = false;
                }
                fprintf(out, "(%d, %.1f) ", ((Entry) get(M->matrix[i]))->column, ((Entry) get(M->matrix[i]))->data);
                seen = true;
            }
            if (seen){
                fprintf(out, "\n");
            }
        }
        // fflush(out);
    }
}