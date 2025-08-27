/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA4
* ListTest.c
* Testing file for modified List ADT  
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"


typedef struct TestStruct{
   int data;
   bool flag;
} TestStruct;

int main(int argc, char* argv[]){

    List A = newList();
    List B = newList();
    TestStruct arr[50]; 

    for (int i = 0, j = 0; i < 50; i++, j+=3) {
        arr[i].data = i * 2 - 7 + j;
        arr[i].flag = j % 2;
        append(A, &arr[i]);
        prepend(B, &arr[i]);
    }

// My own test cases -------------------------------

    append(A, &(arr[1]));
    append(A, &(arr[2]));
    prepend(A, &(arr[3]));

    moveBack(A);
    printf("%d\n", index(A));

    deleteBack(A);
    printf("%d\n", index(A));

    moveBack(A);
    printf("%d\n", index(A));

    prepend(A, &arr[9]);
    printf("%d\n", index(A));

    moveNext(A);
    printf("%d\n", index(A));
    moveNext(A);
    printf("%d\n", index(A));
    printf("%p\n", front(A));
    printf("%p\n", back(A));
    moveFront(A);
    moveNext(A);
    delete(A);
    printList(stdout, A);
    printf("\n");
    printf("%d\n", index(A));
    printf("%p\n", front(A));
    printf("%p\n", back(A));
    clear(A);
    append(A, &arr[18]);
    append(B, &arr[19]);
    append(B, &arr[20]);
    append(B, &arr[21]);
    printf("%d\n", ((TestStruct*)back(B))->data);
    List C = newList();
    for (int i = 49; i >= 0; i--){
        append(C, &(arr[i]));
    }
    printf("%p\n", front(C));
    printf("%p\n", back(C));
    moveBack(C);
    movePrev(C);
    printf("%d\n", ((TestStruct*)get(C))->data);
    printf("%d\n", index(C));
    moveFront(C);
    insertBefore(C, &arr[37]);
    printf("%p\n", front(C));
    printf("%p\n", get(C));
    printf("%d\n", ((TestStruct*)get(C))->data);
    printf("%d\n", index(C));
    moveBack(C);
    insertAfter(C, &(arr[28]));
    printf("%p\n", back(C));
    printf("%p\n", get(C));
    printf("%d\n", ((TestStruct*)get(C))->data);
    printf("%d\n", index(C));
    set(C, &(arr[14]));
    printf("%p\n", get(C));
    delete(C);
    printf("%d\n", index(C));
    moveFront(C);
    moveNext(C);
    moveNext(C);
    deleteFront(C);
    deleteBack(C);
    printf("%d\n", index(C));
    printList(stdout, C);
    printf("\n");



    freeList(&A);
    freeList(&B);
    freeList(&C);
    

    return(0);
}