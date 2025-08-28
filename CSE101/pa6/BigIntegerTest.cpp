/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA6
* BigIntegerTest.cpp
* C++ testing file for BigInteger ADT
***/ 
//-----------------------------------------------------------------------------
//  BigIntegerTest.cpp 
//  A test client for the BigInteger ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<stdexcept>
#include"BigInteger.h"

using namespace std;

int main(){

   long A = 91297236478;
   string B = "-91294213821937";
   string C = "+91294213821937";
   string D = "91294213821937";
   BigInteger iA(A);
   BigInteger iB(B);
   BigInteger iC(C);
   BigInteger iD(D);
   cout << "iA = " << iA << endl;
   cout << "iB = " << iB << endl;
   cout << "iC = " << iC << endl;
   cout << "iD = " << iD << endl;

   long n1 = 99;
   long n2 = 999;
   BigInteger nA(n1);
   BigInteger nB(n2);
   cout << "nA - nB = " << nA - nB << endl;

   BigInteger nC(0);
   BigInteger nD("-2349280498230488842093840923840923849");
   BigInteger nE("+20923849");

   cout << "0 * nA = " << nC * nA << endl;
   cout << "nA * 0 = " << nA * nB << endl;
   cout << "0 - 0 = " << nC - nC << endl;
   cout << "nD - nE = " << nD - nE << endl;

    BigInteger z(000000000000);
    BigInteger zs("00000000000000");
    BigInteger pN("123456789");
    BigInteger nN("-987654321");

    cout << "0 + 0 = " << z + z << endl;
    cout << "0 - 0 = " << z - zs << endl;
    cout << "0 * 0 = " << zs * z << endl;
    cout << "0 + pN = " << z + pN << endl;
    cout << "pN - 0 = " << pN - zs << endl;
    cout << "nN * 0 = " << nN * z << endl;
    cout << "0 * nN = " << z * nN << endl;

    BigInteger nF("-123456");
    BigInteger nG("-789012");

    cout << "nF * nG = " << nF * nG << endl;
    cout << "nF * nG = " << nF * nG << endl;

    BigInteger nH("123456789123456789123456789123456789123456789");
    BigInteger nI("+987654321987654321987654321987654321987654321");

    cout << "nH + nI = " << nH + nI << endl;
    cout << "nH * nI = " << nH * nI << endl;

    BigInteger nJ("13948322308371937");
    nJ *= nG;
    cout << "nJ * nG = " << nJ << endl;
    nJ += nG;
    cout << "nJ + nG = " << nJ << endl;
    nJ -= nG;
    cout << "nJ - nG = " << nJ << endl;

    




   return EXIT_SUCCESS;
}