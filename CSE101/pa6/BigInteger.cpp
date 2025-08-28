//-----------------------------------------------------------------------------
// BigInteger.cpp
// Implementation file for BigInteger ADT
//-----------------------------------------------------------------------------
#include "List.h"
#include "BigInteger.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

const int POWER = 9;
const long BASE = 1000000000;

// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger() {
    signum = 0;
    digits.moveFront();
}

// BigInteger()
// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x){
    if (x < 0){
        x *= -1;
        signum = -1;
    }
    else if (x > 0){
        signum = 1;
    }
    else{
        signum = 0;
    }
    digits.moveFront();
    int currBase = BASE;
    while (x > 0){
        digits.insertBefore(x%currBase);
        x /= BASE;
    }

}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s){
    if (s.length() == 0){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    signum = 1;
    if (s[0] == '+'){
        signum = 1;
        s = s.substr(1);
    }
    else if (s[0] == '-'){
        signum = -1;
        s = s.substr(1);
    }
    if (s.length() == 0){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    for (size_t i = 0; i < s.length(); i++){
        if (s[i] > '9' || s[i] < '0'){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
    }
    long i = static_cast<long>(s.length()) - POWER;
    digits.moveFront();

    while (i >= 0) {
        long num = std::stol(s.substr(static_cast<size_t>(i), POWER));
        digits.insertBefore(num);
        i -= POWER;
    }

    if (i + POWER > 0) {
        long num = std::stol(s.substr(0, static_cast<size_t>(i + POWER)));
        digits.insertBefore(num);
    }

    digits.moveBack();
    long elem = digits.movePrev();
    while (digits.position() > 0 && elem == 0){
        digits.eraseAfter();
        elem = digits.movePrev();
    }

    if (digits.length() == 1 && digits.front() == 0){
        digits.clear();
        signum = 0;
    }
    

}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
    digits = N.digits;
    signum = N.signum;
}

// Optional Destuctor
// ~BigInteger()
// ~BigInteger();


// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() const{
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const{
    if (sub(N).sign() == -1){
        return -1;
    }
    else if (sub(N).sign() == 1){
        return 1;
    }
    else{
        return 0;
    }
}


// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
    digits.clear();
    signum = 0;
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
    if (signum != 0){
        signum *= -1;
    }
}

// Helper Functions --------------------------------------------------------

// negateList() 
// Changes the sign of each integer in List L. Used by sub(). 
void negateList(List& L){
    L.moveFront();
    long elem;
    while (L.position() < L.length()){
        elem = L.moveNext();
        L.setBefore(elem * -1);
    }
}
// normalizeList() 
// Performs carries from left to right (least to most significant digits), 
// then returns the sign of the resulting integer. 
// Used by add(), sub().
int normalizeList(List& L){
    int output = 1;
    L.moveFront();
    long elem;
    long carry = 0;
    while (L.position() < L.length()){
        elem = L.moveNext();
        elem += carry;
        if (elem >= BASE){
            carry = elem / BASE;
            L.setBefore(elem % BASE);
        }
        else if (elem < 0){
            carry = elem / BASE - (elem % BASE != 0);
            L.setBefore((elem % BASE + BASE) % BASE);
            // std::cout << -9 % 100 << ": ELEM % BASE\n";
        }
        else{
            carry = 0;
            L.setBefore(elem);
        }
    }
    if (carry != 0){
        L.insertAfter(carry);
    }
    if (L.back() < 0){
        // std::cout << L << "INSIDENORMALIZE\n";
        output = -1;
        negateList(L);
        normalizeList(L);
    }
    L.moveBack();
    elem = L.movePrev();
    while (L.position() > 0 && elem == 0){
        L.eraseAfter();
        elem = L.movePrev();
    }
    if (L.length() == 1 && L.front() == 0){
        output = 0;
    }
    
    return output;
}

// shiftList() 
// Prepends p zero digits to L, multiplying L by base^p. Used by mult(). 
void shiftList(List& L, int p){
    L.moveFront();
    for (int i = 0; i < p; i++){
        L.insertBefore(0);
    }
}

// scalarMultList() 
// Multiplies L (considered as a vector) by m. Used by mult(). 
void scalarMultList(List& L, ListElement m){
    L.moveFront();
    long elem;
    while (L.position() < L.length()){
        elem = L.moveNext();
        L.setBefore(elem * m);
    }
}

// Alternative implementation using binary search for quotient digit
// This can be more efficient for large numbers
long findQuotientDigit(const BigInteger& remainder, const BigInteger& divisor) {
    if (remainder < divisor) return 0;
    
    long low = 1, high = BASE - 1; // BASE is typically 10^9
    long result = 0;
    
    while (low <= high) {
        long mid = (low + high) / 2;
        BigInteger product;
        product.digits = divisor.digits;
        scalarMultList(product.digits, mid);
        normalizeList(product.digits);
        
        if (remainder >= product) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    return result;
}

// abs()
// Returns the absolute value of this
BigInteger abs(const BigInteger& A){
    BigInteger out = A;
    if (A.signum == 1) return out;
    if (A.signum == -1){
        out.negate();
        return out;
    }
    return out;
}       

// BigInteger Arithmetic operations ----------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const{
    BigInteger A;
    if (signum == 0 && N.signum == 0){
        return A;
    }
    List F = digits;
    List S = N.digits;
    if (signum == -1){
        negateList(F);  
    }
    if (N.signum == -1){
        negateList(S);
    }
    List O;
    int maxLength = 0;
    if (digits.length() > N.digits.length()){
        maxLength = digits.length();
    }
    else{
        maxLength = N.digits.length();
    }
    F.moveFront();
    S.moveFront();
    O.moveFront();
    int p = 0;
    long elem, elem2;
    // std::cout << F << ": F\n";
    // std::cout << S << ": S\n"; 
    while (p < maxLength){
        if (F.position() < F.length()){
            elem = F.moveNext();
        }
        else{
            elem = 0;
        }
        if (S.position() < S.length()){
            elem2 = S.moveNext();
        }
        else{
            elem2 = 0;
        }
        p += 1;
        // std::cout << "HERE\n"; 

        O.insertBefore(elem + elem2);
    }
    // std::cout << O << "INSIDEoutput: \n";
    // std::cout << O << "INSIDEoutput digits: \n";
    int sign = normalizeList(O);
    A.digits = O;
    A.signum = sign;
    return A;
  
}
    

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const{
    List S = N.digits;
    BigInteger A = N;
    negateList(S);
    A.digits = S;
    return add(A);
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const{
    BigInteger output;
    if (N.signum == 0 || signum == 0){
        return output;
    }
    long elem;
    int p = 0;
    List O = N.digits;
    O.moveFront();
    BigInteger O1 = *this;
    O1.digits.moveFront();
    while (O.position() < O.length()){
        // Reset O1 digits because we performed scalar multiplication in the previous iter.
        O1.digits = digits;
        elem = O.moveNext();
        // Multiply by current digit
        scalarMultList(O1.digits, elem);
        // Shift left by current position
        shiftList(O1.digits, p);
        p += 1;
        // Add to result
        output = output.add(O1);
    }

    // Determine sign of result
    if ((signum == -1 && N.signum == -1) || (signum == 1 && N.signum == 1)){
        output.signum = 1;
    }
    else if ((signum == 1 && N.signum == -1) || (signum == -1 && N.signum == 1)){
        output.signum = -1;
    }
    else{
        output.signum = 0;
    }
    
    return output;
}

// div()
// Returns a BigInteger representing the integer division of this and N. 
BigInteger BigInteger::div(const BigInteger& N) const {
    if (N.signum == 0) {
        throw std::invalid_argument("Division by zero");
    }
    
    if (signum == 0) {
        return BigInteger();
    }   
    
    BigInteger dividend = *this;
    dividend.signum = 1;
    BigInteger divisor = N;
    divisor.signum = 1;
    
    BigInteger quotient;
    BigInteger remainder;
    remainder.digits.moveFront();
    remainder.digits.insertAfter(0);
    
    List dividendDigits = dividend.digits;
    dividendDigits.moveBack();
    
    while (dividendDigits.position() > 0) {
        long nextDigit = dividendDigits.movePrev();
        
        if (remainder.signum != 0) {
            remainder.digits.moveFront();
            remainder.digits.insertAfter(0);
        }
        remainder.digits.moveFront();
        remainder.digits.setAfter(nextDigit);
        remainder.signum = 1;
        normalizeList(remainder.digits);
        
        // Use binary search to find quotient digit
        long quotientDigit = findQuotientDigit(remainder, divisor);
        
        if (quotientDigit > 0) {
            BigInteger product;
            product.digits = divisor.digits;
            scalarMultList(product.digits, quotientDigit);
            normalizeList(product.digits);
            remainder = remainder - product;
        }
        
        quotient.digits.moveFront();
        quotient.digits.insertAfter(quotientDigit);
    }
    
    normalizeList(quotient.digits);

    // Determine sign of result
    if ((signum == -1 && N.signum == -1) || (signum == 1 && N.signum == 1)){
        quotient.signum = 1;
    }
    else if ((signum == 1 && N.signum == -1) || (signum == -1 && N.signum == 1)){
        quotient.signum = -1;
    }
    else{
        quotient.signum = 0;
    }

    if (quotient == 0) quotient.signum = 0;
    
    return quotient;
}

// mod()
// Returns a BigInteger representing the remainder of this divided by N.
BigInteger BigInteger::mod(const BigInteger& N) const {
    BigInteger dividend = *this;
    BigInteger divisor = N;
    BigInteger remainder = dividend - (dividend / divisor) * divisor;
    if (remainder.sign() < 0) remainder = remainder + abs(divisor);
    return remainder;
}

// pow()
// Returns a BigInteger representing this to the power of N. This function only supports positive exponents.
BigInteger BigInteger::pow(const BigInteger& N) const {
    if (N.signum == -1) {
        throw std::invalid_argument("Negative exponent");
    }
    BigInteger out(1);
    BigInteger base = *this;
    BigInteger exp = N;
    // Use exponentiation by squaring for efficiency
    while (exp > BigInteger(0)){
        // If exponent is odd, multiply result by current base
        if (exp % BigInteger(2) == BigInteger(1)){
            out *= base; 
        }
        // Square the base and halve the exponent
        base *= base;
        exp /= BigInteger(2);
    }
    return out;
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
    std::string s = "";
    if (signum == 0){
        return "0";
    }
    if (signum == -1){
        s += "-";
    }
    long elem;
    digits.moveBack();
    bool firstIter = true;
    while (digits.position() > 0){
        elem = digits.movePrev();
        std::string num = std::to_string(elem);
        if (!firstIter && static_cast<int>(num.length()) < POWER){
            for (int i = 0; i < POWER - static_cast<int>(num.length()); i++){
                s += "0";
            }
        }
        firstIter = false;
        s += num;
    }
    return s;

}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    // std::cout << N.digits << "\n";
    return stream << N.to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    }
    return false;
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1){
        return true;
    }
    return false;
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0 || A.compare(B) == -1){
        return true;
    }
    return false;
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1){
        return true;
    }
    return false;
}  
    

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0 || A.compare(B) == 1){
        return true;
    }
    return false;
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A.add(B);
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A.sub(B);
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A;
}

// operator/()
// Returns the quotient A/B. 
BigInteger operator/( const BigInteger& A, const BigInteger& B ){
    return A.div(B);
}

// operator/=()
// Overwrites A with the quotient A/B. 
BigInteger operator/=( BigInteger& A, const BigInteger& B ){
    A = A.div(B);
    return A;
}

// operator%()
// Returns the remainder A%B. 
BigInteger operator%( BigInteger& A, const BigInteger& B ){
    return A.mod(B);
}

// operator%=()
// Overwrites A with the remainder A%B. 
BigInteger operator%=( BigInteger& A, const BigInteger& B ){
    A = A.mod(B);
    return A;
}

// operator^()
// Returns A raised to the power of B.
BigInteger operator^( const BigInteger& A, const BigInteger& B ){
    return A.pow(B);
}

// operator^=()
// Overwrites A with A raised to the power of B.
BigInteger operator^=( BigInteger& A, const BigInteger& B ){
    A = A.pow(B);
    return A;
}

BigInteger& BigInteger::operator=(const BigInteger& N) {
    if (this != &N) {
        this->digits = N.digits;
        this->signum = N.signum;
    }
    return *this;
}
