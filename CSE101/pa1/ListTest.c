/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA1 
* ListTest.c
* Testing file for List ADT 
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"List.h"

// From ListClient.c ---------------------------
int main(int argc, char* argv[]){
    List A = newList();
    List B = newList();
    List C = NULL;
    int i;

    for(i=1; i<=20; i++){
        append(A,i);
        prepend(B,i);
    }

    printList(stdout,A); 
    printf("\n");
    printList(stdout,B); 
    printf("\n");

    for(moveFront(A); index(A)>=0; moveNext(A)){
        printf("%d ", get(A));
    }
    printf("\n");
    for(moveBack(B); index(B)>=0; movePrev(B)){
        printf("%d ", get(B));
    }
    printf("\n");

    C = copyList(A);
    printf("%s\n", equals(A,B)?"true":"false");
    printf("%s\n", equals(B,C)?"true":"false");
    printf("%s\n", equals(C,A)?"true":"false");

    moveFront(A);
    for(i=0; i<5; i++) moveNext(A); // at index 5
    insertBefore(A, -1);            // at index 6
    for(i=0; i<9; i++) moveNext(A); // at index 15
    insertAfter(A, -2);
    for(i=0; i<5; i++) movePrev(A); // at index 10
    delete(A);
    printList(stdout,A);
    printf("\n");
    printf("%d\n", length(A));
    clear(A);
    printf("%d\n", length(A));

    freeList(&A);
    freeList(&B);
    freeList(&C);
// My own test cases -------------------------------
    A = newList();
    // Test with 3, movePrev and moveNext functionality using get()
    append(A, 1);
    append(A, 2);
    prepend(A, 3);
    // 3 1 2

    printList(stdout, A);
    printf("\n");

    moveBack(A);
    printf("%d\n", index(A));
    // Cursor at 3, index at 2

    deleteBack(A);
    printf("%d\n", index(A));
    // 3 1, index at -1

    moveBack(A);
    printf("%d\n", index(A));
    // Cursor at 1, index at 1

    prepend(A, 3);
    printList(stdout, A);
    printf("\n");
    printf("%d\n", index(A));
    // 3 3 1
    // index at 2

    moveNext(A);
    printf("%d\n", index(A));
    moveNext(A);
    printf("%d\n", index(A));
    printf("%d\n", front(A));
    printf("%d\n", back(A));
    moveFront(A);
    moveNext(A);
    delete(A);
    printList(stdout, A);
    printf("\n");
    printf("%d\n", index(A));
    printf("%d\n", front(A));
    printf("%d\n", back(A));
    clear(A);
    append(A, 1);
    B = newList();
    append(B, 1);
    append(B, 2);
    append(B, 3);
    printList(stdout, A);
    printf("\n");
    printList(stdout, B);
    printf("\n");
    C = concatList(A, B);
    printList(stdout, C);
    printf("\n");
    printf("%d\n", front(C));
    printf("%d\n", back(C));
    moveBack(C);
    movePrev(C);
    printf("%d\n", get(C));
    printf("%d\n", index(C));
    moveFront(C);
    insertBefore(C, 0);
    printList(stdout, C);
    printf("\n");
    printf("%d\n", front(C));
    printf("%d\n", get(C));
    printf("%d\n", index(C));
    moveBack(C);
    insertAfter(C, 4);
    printList(stdout, C);
    printf("\n");
    printf("%d\n", back(C));
    printf("%d\n", get(C));
    printf("%d\n", index(C));
    set(C, 200);
    printList(stdout, C);
    printf("\n");
    printf("%d\n", get(C));
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

/*
Output of this program:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
false
false
true
1 2 3 4 5 -1 6 7 8 9 11 12 13 14 15 -2 16 17 18 19 20
21
0
3 1 2
2
-1
1
2
-1
-1
3
1
3 1
-1
3
1
1
1 2 3
1 1 2 3
1
3
2
-1
0 1 1 2 3
0
1
1
0 1 1 2 3 4
4
3
4
0 1 1 2 200 4
200
-1
1
1 1 2
*/
