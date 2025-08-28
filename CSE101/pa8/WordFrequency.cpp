/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA8
* WordFrequency.cpp
* C++ file for Dictionary ADT
***/ 
//-----------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include<string>
#include "Dictionary.h"

using namespace std;

#define MAX_LEN 300

void lower(std::string &s){
    for (size_t i = 0; i < s.length(); i++){
        s[i] = tolower(s[i]);
    }
}

int main(int argc, char * argv[]){

   int token_count, line_count;
   size_t begin, end, len;
   ifstream in;
   ofstream out;
   string line;
   string tokenBuffer;
   string token;
   string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$^&%*()-_=+0123456789";

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
      len = line.length();
      
      // get tokens in this line
      token_count = 0;
      tokenBuffer = "";

      // get first token
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = line.substr(begin, end-begin);
      
      while( token!="" ){  // we have a token
         // update token buffer
         tokenBuffer += "   "+token+"\n";
         token_count++;
         lower(token);
         if (A.contains(token)){
            A.getValue(token) += 1;
         }
         else{
            A.setValue(token, 1);
         }

         // get next token
         begin = min(line.find_first_not_of(delim, end+1), len);
         end   = min(line.find_first_of(delim, begin), len);
         token = line.substr(begin, end-begin);
         
      }
   }

   out << A;
   // close files 
   in.close();
   out.close();

   return(EXIT_SUCCESS);
}
