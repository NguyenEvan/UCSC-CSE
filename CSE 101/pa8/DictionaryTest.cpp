/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA8
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
        "langsat", "kaffirlime", "hackberry", "cloudberry", "dewberry", "flacourtia", "gac", "icecreambean", "cranberry",
        "thimbleberry", "starfruit", "plumcot", "mandarine", "riberry", "yangmei", "sapodilla", "vitaminfruit", "uppuma", "quandong",
        "quartfruit", "roseapple", "orangemelon", "tomato", "pear", "zinfandelgrape", "wineberry", "ume", "wolfberry", "voavanga",
        "yellowplum", "ximeniaberry", "waxapple", "vitaminfruit", "zapote", "barberry", "emblica", "dragonfruit", "fingerlime",
        "grapefruit", "honeyberry", "italianlemon", "juneberry", "kiwano", "lucuma", "nut", "ospemifruit", "acai", "vitaminfruit"
    };

    string S2[] = {
        "acai", "acai", "apple", "apricot", "banana", "barberry", "bilberry", "blueberry", "cantaloupe", "cherry",
        "cloudberry", "cranberry", "date", "dewberry", "dragonfruit", "durian", "elderberry", "elderberry", "emblica", "entawak",
        "feijoa", "fig", "flacourtia", "fingerlime", "gac", "grape", "grapefruit", "guava", "hackberry", "honeyberry",
        "honeydew", "icecreambean", "italianlemon", "jaboticaba", "jackfruit", "juneberry", "kaffirlime", "kiwano", "kiwi", "kumquat",
        "langsat", "lemon", "lime", "lucuma", "mandarine", "mango", "medlar", "mulberry", "nectarine", "nut",
        "nutmeg", "olive", "orangemelon", "orange", "ospemifruit", "papaya", "peach", "pear", "plumcot", "quartfruit",
        "quandong", "quararibea", "quince", "rambutan", "raspberry", "riberry", "roseapple", "salak", "sapodilla", "starfruit",
        "strawberry", "tamarillo", "tangerine", "thimbleberry", "tomato", "ume", "uppuma", "vanilla", "vitaminfruit", "vitaminfruit",
        "vitaminfruit", "voavanga", "voavanga", "waxapple", "waxapple", "wineberry", "wolfberry", "xigua", "ximeniaberry", "ximenia",
        "yangmei", "yellowplum", "yuzu", "zapote", "zinfandelgrape", "ziziphus", "zucchini"
    };

    
    Dictionary A;
    Dictionary C;
    for (long unsigned int i = 0; i < sizeof(S) / sizeof(S[0]); i++){
        A.setValue(S[i], i*2-23+23%6*89%9+89-67);
    }
   
    for (long unsigned int i = 0; i < sizeof(S2) / sizeof(S2[0]); i++){
        C.setValue(S2[i], i*2-23+23%6*89%9+89-67);
    }

    cout << A << endl;
    cout << C << endl;
    Dictionary B = A;
    bool equal = A == B;
    bool equal2 = A == C;
    bool equal3 = A.pre_string() == C.pre_string();
    cout << equal << endl;
    cout << equal2 << endl;
    cout << equal3 << endl;
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