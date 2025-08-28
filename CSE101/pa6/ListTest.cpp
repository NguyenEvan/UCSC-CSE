/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA6
* ListTest.c
* C file for List ADT 
***/ 
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

using namespace std;

int main()
{
    List A;
    cout << A << endl;
    for (int i = 1; i <= 50; i+=2){
        A.insertAfter((i * 7 + 7) / 5);
    }
    cout << A << endl;
    A.moveFront();
    cout << A.position() << endl;
    A.moveNext();
    cout << A.position() << endl;
    cout << A.peekNext() << endl;

    A.eraseBefore();
    A.setAfter(4);
    A.findNext(3);
    A.setBefore(21);
    cout << A << endl;

    cout << A.length() << endl;
    cout << A.front() << endl;
    cout << A.back() << endl;

    A.moveBack();
    cout << A.position() << endl;
    A.findPrev(42);
    A.movePrev();
    A.movePrev();
    A.findNext(11);
    cout << A.position() << endl;
    cout << A.peekNext() << endl;
    cout << A.peekPrev() << endl;

    List B = A;
    cout << B << endl;

    cout << (A == B ? "true" : "false") << endl;

    A.clear();
    cout << A << endl;

    // More insertion and testing
    for (int i = 1; i <= 7; i+=6) {
        B.insertBefore(i - 7);
        A.insertAfter(i);
    }
    cout << B << endl;

    List C = A.concat(B);
    cout << C << endl;

    C.insertAfter(2);
    C.insertAfter(2);
    C.insertAfter(5);
    C.moveNext();
    C.insertBefore(-31);
    cout << C << endl;
    C.cleanup();
    cout << C << endl;
    C.eraseAfter();
    C.eraseBefore();
    cout << C << endl;

    List D;
    D.insertBefore(2);
    D.insertBefore(2);
    D.insertBefore(1);
    D.insertBefore(2);
    D.insertBefore(1);
    D.insertBefore(1);
    D.insertBefore(3);
    D.insertBefore(2);
    D.insertBefore(3);
    D.insertBefore(3);
    D.insertBefore(1);
    D.insertBefore(2);
    D.moveFront();
    D.moveNext();
    D.moveNext();
    D.moveNext();
    D.moveNext();
    cout << D << endl;
    cout << D.position() << endl;
    D.cleanup();
    cout << D << endl;
    cout << D.position() << endl;

    List E;
    E.insertBefore(167686786);
    E.insertBefore(5765652);
    E.insertBefore(2768676);
    E.insertBefore(76556751);
    E.insertBefore(657562);
    E.insertBefore(657657653);
    E.insertBefore(756756751);
    E.findPrev(657562);
    E.movePrev();
    cout << E << endl;
    cout << E.position() << endl;
    E.cleanup();
    cout << E << endl;
    cout << E.position() << endl;
    
    
    

    return (EXIT_SUCCESS);
}