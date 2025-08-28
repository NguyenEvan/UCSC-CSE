/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4
* MatrixTest.c
* Testing file for Matrix ADT 
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"
#include "Matrix.h"

int main(int argc, char* argv[]){
    int n = 1000;
    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);
    for (int i = 1; i <= 100; i++){
      changeEntry(A, i*5, i+1*4, i*89.0/6.87+234);
      changeEntry(B, i*2-1, i+1*3, i-6.87+234);
    }
    Matrix cancel = newMatrix(n);
    for (int i = 1; i <= 10; i++){
      changeEntry(cancel, i*5, i+5*2, -1);
    }
    Matrix C = sum(A, A);
    Matrix D = scalarMult(2, A);
    printf("%d\n", equals(C, D));
    printf("%d\n", size(A));
    printf("%d\n", size(B));
    printf("%d\n", size(C));
    printf("%d\n", size(D));
    printf("%d\n", NNZ(A));
    printf("%d\n", NNZ(B));
    printf("%d\n", NNZ(C));
    printf("%d\n", NNZ(D));
    Matrix E = product(cancel, D);
    printMatrix(stdout, E);
    Matrix F = copy(cancel);
    printMatrix(stdout, F);
    Matrix G = transpose(cancel);
    printMatrix(stdout, G);
    Matrix H = transpose(G);
    printf("%d\n", equals(H, cancel));
    makeZero(H);
    Matrix I = diff(H, cancel);
    printMatrix(stdout, I);
    Matrix J = sum(C, A);
    Matrix K = scalarMult(3, A);
    printf("%d\n", equals(J, K));
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
    freeMatrix(&K);
    freeMatrix(&cancel);
    

    return(0);
}