/***
* Evan Nguyen
* evbnguye
* 2024 Fall CSE101 PA8
* Dictionary.cpp
* C++ file for Dictionary ADT 
***/ 
//-----------------------------------------------------------------------------
// Dictionary.cpp
// Implementation file for Dictionary ADT
//-----------------------------------------------------------------------------
#include "Dictionary.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

// Helper Functions (Optional) ---------------------------------------------

// transplant()
// Replaces the given subtree rooted at u with the subtree rooted at v
void Dictionary::RB_Transplant(Node* u, Node* v){
    if (u->parent == nil){
        root = v;
    }
    else if (u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    
    v->parent = u->parent;
    
}
// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const{
    if (R != nil){
        inOrderString(s, R->left);
        s.append(R->key + " : " + std::to_string(R->val) + "\n");
        inOrderString(s, R->right);
    }

}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string consists of keys only, separated by "\n", with the order determined
// by a pre-order tree walk.
void Dictionary::preOrderString(std::string& s, Node* R) const{
    if (R != nil){
        if (R->color == 1){
            s.append(R->key + " (RED)\n");
        }
        else{
            s.append(R->key + "\n");
        }
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// BST_insert()
// Inserts a copy of the Node *M into this Dictionary. Used by preOrderCopy().
void Dictionary::BST_insert(Node* M){
    num_pairs += 1;
    Node* z = new Node(M->key, M->val);
    z->left = nil;
    z->right = nil;
    z->color = M->color;
    Node* y = nil;
    Node* x = root;
    while (x != nil){
        y = x;
        if (z->key < x->key){
            x = x->left;
        }
        else{ 
            x = x->right;
        }
    }
    z->parent = y;
    if (y == nil){             // T was empty
        root = z;
    }    
    else if (z->key < y->key){
        y->left = z;
    }
    else{ 
        y->right = z;
    }
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N){
    if (R != nil && R != N){
        // std::cout << "key: " << R->key << ", value: " << R->val << "\n";
        BST_insert(R);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R){
    if (R != nil){
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }

}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
    if (R == nil || k == R->key){
        return R;
    }
    else if (k < R->key){
        return search(R->left, k);
    }
    else { // k > x.key
        return search(R->right, k);
    }
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R){
    while (R->left != nil){
        R = R->left;
    }
    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R){
    while (R->right != nil){
        R = R->right;
    }
    return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N){
    if (N->right != nil){
        return findMin(N->right);
    }
    Node* y = N->parent;
    while (y != nil && N == y->right){
        N = y;
        y = y->parent;
    }
    return y;
}


// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N){
    if (N->left != nil){
        return findMax(N->left);
    }
    Node* y = N->parent;
    while (y != nil && N == y->left){
        N = y;
        y = y->parent;
    }
    return y;
}

// RBT Helper Functions (Optional) -----------------------------------------

// LeftRotate()
void Dictionary::LeftRotate(Node* N){
    // set y
    Node* y = N->right;
    
    // turn y's left subtree into x's right subtree
    N->right = y->left; 
    y->left->parent = N;
    
    // link y's parent to x
    y->parent = N->parent;
    if (N->parent == nil){
        root = y;
    }
    else if (N == N->parent->left){
        N->parent->left = y;
    }
    else{ 
        N->parent->right = y;
    }
    
    // put x on y's left
    y->left = N; 
    N->parent = y;
}

// RightRotate()
void Dictionary::RightRotate(Node* N){
    // set y
    Node* y = N->left; 
    
    // turn y's right subtree into x's left subtree
    N->left = y->right; 
    y->right->parent = N;
    
    // link y's parent to x
    y->parent = N->parent;
    if (N->parent == nil){
        root = y;
    }
    else if (N == N->parent->right){
        N->parent->right = y;
    }
    else{ 
        N->parent->left = y;
    }
    // put x on y's right
    y->right = N;
    N->parent = y;
}

// RB_InsertFixUP()
void Dictionary::RB_InsertFixUp(Node* N){
    while (N->parent->color == 1){
        if (N->parent == N->parent->parent->left){
            Node* y = N->parent->parent->right;
            if (y->color == 1){
                N->parent->color = 0;            // case 1
                y->color = 0;                   // case 1
                N->parent->parent->color = 1;         // case 1
                N = N->parent->parent;                 // case 1
            }
            else{ 
                if (N == N->parent->right){
                    N = N->parent;                    // case 2
                    LeftRotate(N);                  // case 2
                }                
                N->parent->color = 0;            // case 3
                N->parent->parent->color = 1;         // case 3
                RightRotate(N->parent->parent);     // case 3
            }
        }
        else{ 
            Node* y = N->parent->parent->left;
            if (y->color == 1){
                N->parent->color = 0;            // case 4
                y->color = 0;                   // case 4
                N->parent->parent->color = 1;         // case 4
                N = N->parent->parent  ;               // case 4
            }
            else{ 
                if (N == N->parent->left){
                    N = N->parent;                     // case 5
                    RightRotate(N);               // case 5
                }
                N->parent->color = 0;            // case 6
                N->parent->parent->color = 1;         // case 6
                LeftRotate(N->parent->parent);      // case 6
            }
        }
    }      
   root->color = 0;
}

// // RB_Transplant()
// void Dictionary::RB_Transplant(Node* u, Node* v){

// }

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* N){
    while (N != root && N->color == 0)
        if (N == N->parent->left){
            Node* w = N->parent->right;
            if (w->color == 1){
                w->color = 0;                        // case 1
                N->parent->color = 1;                   // case 1
                LeftRotate(N->parent);                // case 1
                w = N->parent->right;                  // case 1
            }
            if (w->left->color == 0 && w->right->color == 0){
                w->color = 1;                          // case 2
                N = N->parent;                          // case 2
            }                       
            else{ 
                if (w->right->color == 0){
                    w->left->color = 0;                // case 3
                    w->color = 1;                       // case 3
                    RightRotate(w);                   // case 3
                    w = N->parent->right;                  // case 3
                }
                w->color = N->parent->color;               // case 4
                N->parent->color = 0;                 // case 4
                w->right->color = 0;                  // case 4
                LeftRotate(N->parent);                // case 4
                N = root;                             // case 4
            }
        }
        else{ 
            Node* w = N->parent->left;
            if (w->color == 1){
                w->color = 0;                        // case 5
                N->parent->color = 1;                   // case 5
                RightRotate(N->parent);               // case 5
                w = N->parent->left;                      // case 5
            }
            if (w->right->color == 0 && w->left->color == 0){
                w->color = 1;                          // case 6
                N = N->parent;                           // case 6
            }
            else{ 
                if (w->left->color == 0){
                    w->right->color = 0;               // case 7
                    w->color = 1;                       // case 7
                    LeftRotate(w);                    // case 7
                    w = N->parent->left;                   // case 7
                }
                w->color = N->parent->color;               // case 8
                N->parent->color = 0;                 // case 8
                w->left->color = 0;                   // case 8
                RightRotate(N->parent);               // case 8
                N = root;                             // case 8
            }
        }
    N->color = 0;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* N){
    Node* y = N;
    int y_original_color = y->color;
    Node* x = nullptr;
    if (N->left == nil){
        x = N->right;
        RB_Transplant(N, N->right);
    }
    else if (N->right == nil){
        x = N->left;
        RB_Transplant(N, N->left);
    }
    else{ 
        y = findMin(N->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == N){
            x->parent = y;
        }
        else{ 
            RB_Transplant(y, y->right);
            y->right = N->right;
            y->right->parent = y;
        }
        RB_Transplant(N, y);
        y->left = N->left;
        y->left->parent = y;
        y->color = N->color;
    }
    if (x == nullptr){
        std::cout << "Something has gone very wrong!\n";
    }
    else{
        if (y_original_color == 0){
            RB_DeleteFixUp(x);
        }
    }
    delete N;
}

// Private Constructor --------------------------------------------------------

// Node constructor
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = left = right = nullptr;
    color = 0;
}

// Class Constructors & Destructors ----------------------------------------
   
// Creates new Dictionary in the empty state. 
Dictionary::Dictionary(){
    num_pairs = 0;
    nil = new Node("NIL", -999);
    root = nil;
    current = nil;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("NIL", -999);
    root = current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary(){
    postOrderDelete(root);
    // std::cout << "root: " << root->val << ", current: " << current->val << "\n";
    delete nil;
}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const{
    if (search(root, k) == nil){
        return false;
    }
    return true;
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{
    if (contains(k) == false){
        throw std::logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    }
    Node* out = search(root, k);
    return out->val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const{
    return current != nil ? true : false;
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{
    if (hasCurrent() == false){
        throw std::logic_error("Dictionary: currentKey(): current undefined");
    }
    return current->key;
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if (hasCurrent() == false){
        throw std::logic_error("Dictionary: currentVal(): current undefined");
    }
    return current->val;
}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    postOrderDelete(root);
    num_pairs = 0;
    current = root = nil;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){
    Node* y = nil;
    Node* x = root;
    while (k != x->key && x != nil){
        y = x;
        if (k < x->key){
            x = x->left;
        }
        else if (k > x->key){ 
            x = x->right;
        }
    }
    if (k == x->key){
        x->val = v;
    }
    else{
        Node* z = new Node(k, v);
        z->parent = y;
        if (y == nil){
            root = z;
        }
        else if (z->key < y->key){
            y->left = z;
        }
        else{ 
            y->right = z;
        }
        z->left = nil;
        z->right = nil;
        z->color = 1;
        num_pairs += 1;
        RB_InsertFixUp(z);
    }
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k){
    if (contains(k) == false){
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    }
    num_pairs -= 1;
    Node* z = search(root, k);
    if (current == z){
        current = nil;
    }
    RB_Delete(z);
    
}
    
// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin(){
    if (num_pairs != 0){
        current = findMin(root);
    }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end(){
    if (num_pairs != 0){
        current = findMax(root);
    }
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
    if (hasCurrent() == false){
        throw std::logic_error("Dictionary: next(): current undefined");
    }
    current = findNext(current);
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
    if (hasCurrent() == false){
        throw std::logic_error("Dictionary: prev(): current undefined");
    }
    current = findPrev(current);
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const{
    std::string s = "";
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const{
    std::string s = "";
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
    std::string s1 = "";
    std::string s2 = "";
    inOrderString(s1, root);
    D.inOrderString(s2, D.root);
    return s1 == s2;
}


// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    stream << D.to_string();
    return stream;
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    if (this != &D){
        Dictionary temp = D;
        std::swap(nil, temp.nil);
        std::swap(root, temp.root);
        std::swap(current, temp.current);
        std::swap(num_pairs, temp.num_pairs);
    }
    return *this;
}