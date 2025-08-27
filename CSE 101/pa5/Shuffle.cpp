/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA5
* Shuffle.c
* C++ file for List ADT
***/ 
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

using namespace std;

void shuffle(List& D){
    int split_length = D.length() / 2;
    bool odd_len = false;
    D.moveFront();
    List S1, S2;
    for (int i = 0; i < split_length; i++){
        S1.insertBefore(D.moveNext());
    }
    if (D.length() % 2 == 1){
        split_length += 1;
        odd_len = true;
    }
    for (int i = 0; i < split_length; i++){
        S2.insertBefore(D.moveNext());
    }
    D.clear();
    S1.moveFront();
    S2.moveFront();
    if (odd_len){
        split_length -= 1;
    }
    for (int i = 0; i < split_length; i++){
        D.insertBefore(S2.moveNext());
        D.insertBefore(S1.moveNext());
    }
    if (odd_len){
        D.insertBefore(S2.moveNext());
    }

}

int main(int argc, char* argv[])
{
    int length, shuffles;
    length = stoi(argv[1]);
    List D, D_orig;
    cout << "deck size\tshuffle count" << endl;
    cout << "------------------------------" << endl;
    for (int i = 1; i <= length; i++){
        cout << i << "\t\t";
        D.insertBefore(i-1);
        D_orig = D;
        shuffles = 1;
        shuffle(D);
        while (!(D == D_orig)){
            shuffle(D);
            shuffles += 1;
        }
        cout << shuffles << endl;
    }

}