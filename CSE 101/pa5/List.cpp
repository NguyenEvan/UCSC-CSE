/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA5
* List.c
* C++ file for List ADT 
***/ 
//-----------------------------------------------------------------------------
// List.cpp
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include "List.h"

#include <iostream>
#include <stdexcept>
#include <string>

// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List() {
    pos_cursor = 0;
    num_elements = 0;
    backDummy = new Node(-999);
    frontDummy = new Node(-999);
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    backDummy->prev = frontDummy;
    frontDummy->next = backDummy;
}

// Copy Constructor.
List::List(const List &L) {
    // make this an empty List
    this->frontDummy = new Node(-999);
    this->backDummy = new Node(-999);
    this->afterCursor = backDummy;
    this->beforeCursor = frontDummy;
    this->num_elements = 0;
    this->pos_cursor = 0;
    this->backDummy->prev = frontDummy;
    this->frontDummy->next = backDummy;
    Node *N = L.frontDummy->next;

    // load elements of L into this List
    while (N != L.backDummy) {
        this->insertBefore(N->data);
        N = N->next;
    }
    // std::cout << "***called copy constructor: " << this->to_string() <<
    // std::endl;
}

// Destructor
List::~List() {
    moveFront();
    while (num_elements > 0) {
        eraseAfter();
    }
    delete frontDummy;
    delete backDummy;
}

// Access functions -----------------------------------------------------------

// length()
// Returns the length of this List.
int List::length() const{
    return num_elements; 
}

// front()
// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const{
    if (num_elements == 0){
        throw std::length_error("List: front(): empty list");
    }
    return frontDummy->next->data; 
}

// back()
// Returns the back element in this List.
// pre: length()>0
ListElement List::back() const{
    if (num_elements == 0){
        throw std::length_error("List: back(): empty list");
    }
    return backDummy->prev->data; 
}

// position()
// Returns the position of cursor in this List: 0 <= position() <= length().
int List::position() const{
    return pos_cursor; 
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const{
    if (position() >= length()){
        throw std::range_error("List: peekNext(): cursor at back");
    }
    return afterCursor->data;
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const{
    if (position() <= 0){
        throw std::range_error("List: peekPrev(): cursor at front");
    }
    return beforeCursor->data; 
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveFront();
    while (length() > 0){
        eraseAfter();
    }
}

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

// moveBack()
// Moves cursor to position length() in this List.
void List::moveBack() {
    pos_cursor = num_elements;
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
}

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over.
// pre: position()<length()
ListElement List::moveNext(){
    if (position() >= length()){
        throw std::range_error("List: moveNext(): cursor at back");
    }
    ListElement out = afterCursor->data;
    afterCursor = afterCursor->next;
    beforeCursor = beforeCursor->next;
    pos_cursor += 1;
    return out;
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over.
// pre: position()>0
ListElement List::movePrev(){
    if (position() <= 0){
        throw std::range_error("List: movePrev(): cursor at front");
    }
    ListElement out = beforeCursor->data;
    afterCursor = afterCursor->prev;
    beforeCursor = beforeCursor->prev;
    pos_cursor -= 1;
    return out;
}

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(ListElement x){
    Node *N = new Node(x);
    afterCursor->prev = N;
    beforeCursor->next = N;
    N->next = afterCursor;
    N->prev = beforeCursor;
    afterCursor = N;
    num_elements += 1;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(ListElement x){
    Node *N = new Node(x);
    afterCursor->prev = N;
    beforeCursor->next = N;
    N->next = afterCursor;
    N->prev = beforeCursor;
    beforeCursor = N;
    pos_cursor += 1;
    num_elements += 1;
}

// setAfter()
// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x){
    if (position() >= length()){
        throw std::range_error("List: setAfter(): cursor at back");
    }
    afterCursor->data = x;
}

// setBefore()
// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x){
    if (position() <= 0){
        throw std::range_error("List: setBefore(): cursor at front");
    }
    beforeCursor->data = x;
}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter(){
    if (position() >= length()){
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
    beforeCursor->next = afterCursor->next;
    afterCursor->next->prev = beforeCursor;
    num_elements -= 1;
    Node* newAfter = afterCursor->next;
    delete afterCursor;
    afterCursor = newAfter;
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore(){
    if (position() <= 0){
        throw std::range_error("List: eraseBefore(): cursor at front");
    }
    afterCursor->prev = beforeCursor->prev;
    beforeCursor->prev->next = afterCursor;
    num_elements -= 1;
    pos_cursor -= 1;
    Node* newBefore = beforeCursor->prev;
    delete beforeCursor;
    beforeCursor = newBefore;
}

// Other Functions ------------------------------------------------------------

// findNext()
// Starting from the current cursor position, performs a linear search (in
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then
// returns the final cursor position. If x is not found, places the cursor
// at position length(), and returns -1.
int List::findNext(ListElement x){
    for (; position() < length(); moveNext()){
        if (peekNext() == x){
            moveNext();
            return pos_cursor;
        }
    }
    moveBack();
    return -1;
}

// findPrev()
// Starting from the current cursor position, performs a linear search (in
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor
// at position 0, and returns -1.
int List::findPrev(ListElement x){
    for (; position() > 0; movePrev()){
        if (peekPrev() == x){
            movePrev();
            return pos_cursor;
        }
    }
    moveFront();
    return -1;
}

// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost
// occurrance of each element, and removing all other occurances. The cursor
// is not moved with respect to the retained elements, i.e. it lies between
// the same two retained elements that it did before cleanup() was called.
void List::cleanup(){
    // int iter = 0;
    int pos = 0;
    int savedPos = length() - position();
    int numRightDeleted = 0;
    std::cout << "Saved Pos: " << savedPos << "\n";
    for (moveFront(); position() < length();){
        // std::cout << "Iter:" << iter << "\n";
        ListElement curr = moveNext();
        // std::cout << "element: " << curr << "\n";
        while (position() < length()){
            // std::cout << "Pos: " << position() << "\n";
            // std::cout << "Peeking: " << peekNext() << "\n";
            // std::cout << "element: " << curr << "\n";
            bool flag = false;
            if (curr == peekNext()){
                // std::cout << "Erasing..." << "\n";
                flag = true;
                if (length() - position() + numRightDeleted <= savedPos){
                    std::cout << "Erasing..." << length() - position() + numRightDeleted << "\n";
                    // std::cout << "savedPos..." << savedPos << "\n";
                    std::cout << "curr..." << curr << "\n";
                    numRightDeleted += 1;
                }
                eraseAfter();
            }
            if (!flag){
                moveNext();
            }
        }
        // iter += 1;
        moveFront();
        pos += 1;
        // int innerIter = 0;
        // std::cout << *this << "\n";
        // std::cout << position() << "\n";
        for (int i = 0; i < pos; i++){
            // std::cout << "Inner iter:" << innerIter << "\n";
            moveNext();
            // innerIter += 1;
        }
        // std::cout << "after manip: " << position() << "\n";
    }
    for (int i = 0; i < savedPos - numRightDeleted; i++){
        movePrev();
    }
}

// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List &L) const{
    List J;
    Node *N = this->frontDummy->next;
    Node *M = L.frontDummy->next;
    J.moveFront();
    while (N != this->backDummy) {
        J.insertBefore(N->data);
        N = N->next;
    }
    while (M != L.backDummy) {
        J.insertBefore(M->data);
        M = M->next;
    }
    J.moveFront();
    return J;
}

// to_string()
// Returns a string representation of this List consisting of a comma
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const{
    std::string s = "";
    Node *N = frontDummy->next;
    if (length() > 0){
        s += "(";
    }
    while (N != backDummy){
        if (N->next == backDummy){
            s += std::to_string(N->data) + ")";
        }
        else{
            s += std::to_string(N->data) + ", ";
        }
        N = N->next;
    }
    return s;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List &R) const{
    bool eq = false;
    Node *N = this->frontDummy->next;
    Node *M = R.frontDummy->next;
    eq = (this->num_elements == R.num_elements);
    while (eq && N != this->backDummy && M != R.backDummy){
        eq = (N->data == M->data);
        N = N->next;
        M = M->next;
    }
    return eq;
}

// Overloaded Operators --------------------------------------------------------

// operator<<()
// Inserts string representation of L into stream.
std::ostream &operator<<(std::ostream &stream, const List &L){
    return stream << L.to_string();
}

// operator==()
// Returns true if and only if A is the same integer sequence as B. The
// cursors in both Lists are unchanged.
bool operator==(const List &A, const List &B){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List &List::operator=(const List &L){
    if (this != &L){
        List temp = L;
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(num_elements, temp.num_elements);
    }
    return *this;
}