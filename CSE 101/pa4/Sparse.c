/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4
* Sparse.c
* Client for the Matrix ADT; Used to take two equally-sized matrices from an input file
* and perform various matrix operations on one or both of them, including
* addition, scalar multiplication, subtraction, transpose, and multiplication
***/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "List.h"
#include "Matrix.h"

#define MAX_LEN 300

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *in, *out;
    char line[MAX_LEN];
    char *endptr;
    char *ptr;
    bool finishedReading = false;
    int row, column, n, a, b;
    double value;
    Matrix A, B;
    
     // open files for reading and writing 
    in = fopen(argv[1], "r");
    if( in==NULL ){
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    out = fopen(argv[2], "w");
    if( out==NULL ){
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(EXIT_FAILURE);
    }


    bool firstIter = true;
    int manipulateA = 0;
    int manipulateB = 0;
    
    while(!finishedReading && fgets(line, MAX_LEN, in) != NULL)  {
        ptr = line;
        if (!firstIter){
            row = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            column = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            value = (double) strtod(ptr, &endptr);
            ptr = endptr;
            if (manipulateA < a){
                changeEntry(A, row, column, value);
                manipulateA += 1;
                if (manipulateA == a){
                    fgets(line, MAX_LEN, in);
                }
            }
            else{
                if (manipulateB < b){
                    changeEntry(B, row, column, value);
                    manipulateB += 1;
                    if (manipulateB == b){
                        finishedReading = true;
                        break;
                    }
                }
            }
        }
        else{
            n = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            a = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            b = (int) strtol(ptr, &endptr, 10);
            ptr = endptr;
            A = newMatrix(n);
            B = newMatrix(n);
            if (a == 0 && b == 0){
                finishedReading = true;
                break;
            }
            firstIter = false;
            fgets(line, MAX_LEN, in);
        }
        
    }
    
    fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(out, A);
    fprintf(out, "\n");

    fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
    printMatrix(out, B);
    fprintf(out, "\n");

    fprintf(out, "(1.5)*A =\n");
    Matrix C = scalarMult(1.5, A);
    printMatrix(out, C);
    fprintf(out, "\n");

    fprintf(out, "A+B =\n");
    Matrix D = sum(A, B);
    printMatrix(out, D);
    fprintf(out, "\n");

    fprintf(out, "A+A =\n");
    Matrix E = sum(A, A);
    printMatrix(out, E);
    fprintf(out, "\n");

    fprintf(out, "B-A =\n");
    Matrix F = diff(B, A);
    printMatrix(out, F);
    fprintf(out, "\n");

    fprintf(out, "A-A =\n");
    Matrix G = diff(A, A);
    printMatrix(out, G);
    fprintf(out, "\n");

    fprintf(out, "Transpose(A) =\n");
    Matrix H = transpose(A);
    printMatrix(out, H);
    fprintf(out, "\n");

    fprintf(out, "A*B =\n");
    Matrix I = product(A, B);
    printMatrix(out, I);
    fprintf(out, "\n");

    fprintf(out, "B*B =\n");
    Matrix J = product(B, B);
    printMatrix(out, J);


    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&C);
    freeMatrix(&D);
    freeMatrix(&E);
    freeMatrix(&F);
    freeMatrix(&G);
    freeMatrix(&H);
    freeMatrix(&I);
    freeMatrix(&J);
    fclose(in);
    fclose(out);
    return 0;
}