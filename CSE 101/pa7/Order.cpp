/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA7
* Order.cpp
* C++ file for Dictionary ADT
***/ 
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Dictionary.h"

using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

   int line_count;
   ifstream in;
   ofstream out;
   string line;
   //string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$^&*()-_=+0123456789";

   // check command line for correct number of arguments
   if( argc != 3 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }

   // open files for reading and writing 
   in.open(argv[1]);
   if( !in.is_open() ){
      cerr << "Unable to open file " << argv[1] << " for reading" << endl;
      return(EXIT_FAILURE);
   }

   out.open(argv[2]);
   if( !out.is_open() ){
      cerr << "Unable to open file " << argv[2] << " for writing" << endl;
      return(EXIT_FAILURE);
   }

   Dictionary A;

   // read each line of input file, then count and print tokens 
   line_count = 0;
   while( getline(in, line) )  {
      line_count++;
      A.setValue(line, line_count);
   }
   out << A << endl << endl;
   out << A.pre_string() << endl;


   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}