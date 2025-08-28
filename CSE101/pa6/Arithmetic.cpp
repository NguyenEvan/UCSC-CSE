/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA6
* Arithmetic.c
* C++ file for BigInteger ADT
***/ 
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "BigInteger.h"

using namespace std;

int main(int argc, char* argv[])
{
    ifstream ifile(argv[1]); 
    ofstream ofile(argv[2]);
    string line;
    getline(ifile, line);
    BigInteger A = BigInteger(line);
    getline(ifile, line);
    getline(ifile, line);
    BigInteger B = BigInteger(line);

    BigInteger C = A + B;
    BigInteger D = A - B;
    BigInteger E = A - A;
    BigInteger F = 3 * A - 2 * B;
    BigInteger G = A * B;
    BigInteger H = A * A;
    BigInteger I = B * B;
    BigInteger J = (9 * (A * A * A * A)) + (16 * (B * B * B * B * B));
    ofile << A << endl << endl;
    ofile << B << endl << endl; 
    ofile << C << endl << endl; 
    ofile << D << endl << endl; 
    ofile << E << endl << endl; 
    ofile << F << endl << endl;  
    ofile << G << endl << endl; 
    ofile << H << endl << endl;
    ofile << I << endl << endl;
    ofile << J << endl;
    ifile.close();
    ofile.close();
}