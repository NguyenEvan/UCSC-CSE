/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA7
* DictionaryTest.cpp
* C++ testing file for Dictionary ADT
***/ 
//-----------------------------------------------------------------------------
// DictionaryTest.cpp
// A test client for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"

using namespace std;

int main(){
    string S[] =   {
        "kiwi", "quince", "elderberry", "date", "cherry", "banana", "fig", "lemon", "honeydew", "apple",
        "nectarine", "orange", "grape", "papaya", "tangerine", "raspberry", "watermelon", "strawberry", "ugli", "mango",
        "mulberry", "peach", "cantaloupe", "lime", "zucchini", "durian", "olive", "blueberry", "nutmeg", "apricot",
        "entawak", "rambutan", "vanilla", "feijoa", "guava", "jackfruit", "quararibea", "kumquat", "salak", "tamarillo",
        "voavanga", "ugli", "waxapple", "xigua", "ziziphus", "yuzu", "bilberry", "medlar", "ximenia", "jaboticaba",
        "langsat", "kaffirlime", "elderberry", "hackberry", "cloudberry", "dewberry", "flacourtia", "gac", "icecreambean", "cranberry",
        "thimbleberry", "starfruit", "plumcot", "mandarine", "riberry", "yangmei", "sapodilla", "vitaminfruit", "uppuma", "quandong",
        "quartfruit", "roseapple", "orangemelon", "tomato", "pear", "zinfandelgrape", "wineberry", "ume", "wolfberry", "voavanga",
        "yellowplum", "ximeniaberry", "waxapple", "vitaminfruit", "zapote", "acai", "barberry", "emblica", "dragonfruit", "fingerlime",
        "grapefruit", "honeyberry", "italianlemon", "juneberry", "kiwano", "lucuma", "nut", "ospemifruit", "acai", "vitaminfruit"
    };
    
    Dictionary A;
    for (long unsigned int i = 0; i < sizeof(S) / sizeof(S[0]); i++){
        A.setValue(S[i], i*2-23+23%6*89%9+89-67);
    }
    cout << A << endl;
    Dictionary B = A;
    bool equal = A == B;
    cout << equal << endl;
    B.setValue("zucchini", -231341241);
    B.setValue("quince", -3213);
    B.setValue("acai", 99987);
    cout << A.size() << endl;
    cout << B.size() << endl;
    equal = A == B;
    cout << equal << endl;

    for(A.begin(); A.hasCurrent(); A.next()){
      string s = A.currentKey();
      int x = A.currentVal();
      cout << "("+s+", " << x << ") ";
    }
    cout << endl << endl;

    
    for(B.end(); B.hasCurrent(); B.prev()){
        string s = B.currentKey();
        int x = B.currentVal();
        cout << "("+s+", " << x << ") ";
    }
    A.begin();
    A.next();
    B.end();
    B.prev();
    cout << A.currentVal() << endl;
    cout << A.currentKey() << endl;
    cout << B.currentVal() << endl;
    cout << B.currentKey() << endl;
    cout << A.getValue("acai") << endl;
    cout << B.getValue("acai") << endl;
    A.getValue("acai") *= -32;
    cout << A.getValue("acai") << endl;
    B = A;
    equal = A == B;
    cout << equal << endl;
    cout << B.getValue("acai") << endl;
    cout << B.contains("rambutan") << endl;
    B.clear();
    cout << B << endl;
    cout << B.hasCurrent() << endl;
    B.setValue("someone pls read this", 420);
    B.end();
    cout << B.currentVal() << endl;
    cout << B.currentKey() << endl;
    B.begin();
    B.setValue("someone pls read this", 69);
    cout << B.currentVal() << endl;
    cout << B.currentKey() << endl;
    A.remove("acai");
    A.remove("raspberry");
    cout << A.pre_string() << endl;
    cout << B.pre_string() << endl;
    cout << B << endl;







    cout << endl;  

    return( EXIT_SUCCESS );
}