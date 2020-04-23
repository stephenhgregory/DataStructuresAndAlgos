/*
 * Implementation of a Binary Heap.
 * 
 * This file contains two classes:
 * 1. Heap
 * 2. Node
 * 
 * Both of these classes are templated, with
 * two typenames: keytype and valuetype.
 * Therefore, any instantiation of BNode or
 * BHeap must be as follows:
 * 1. template <typename keytype, typename keytype>
 *    Node<keytype, valuetype> this_name;
 * 2. template <typename keytype, typename valuetype>
 *    Heap<keytype, valuetype> that_name;
 * 
 * 
 * @author      Stephen Gregory
 * @date        04/21/2020
 */

#ifndef HEAP_CPP
#define HEAP_CPP

#include <iostream>
#include "CDA.cpp"

// Node is one single element of a CDA
template <typename keytype, typename valuetype>
struct Node {
    keytype key;
    valuetype value;
    Node() {}
    Node(keytype key, valuetype value) : key(key), value(value) {}
    bool operator<(Node const &rhs) { return key < rhs.key; };
    bool operator<=(Node const &rhs) { return key <= rhs.key; };
    bool operator==(Node const &rhs) { return key == rhs.key; };
    bool operator>(Node const &rhs) { return key > rhs.key; };
    bool operator>=(Node const &rhs) { return key >= rhs.key; }
};


// Heap is a Binary Min-Ordered Heap
template <typename keytype, typename valuetype>
class Heap {
public:
    Heap();                                         // Default Constructor for empty Heap 
    Heap(keytype k[], valuetype v[], int s);        // Constructor with keys k[], values v[], size s
    Heap(const Heap &heap);                         // Copy Constructor
    Heap& operator=(const Heap &heap);              // Copy assignment operator

    void insert(keytype k, valuetype v);            // Inserts key k and value v into the Heap
    void siftUp(int node_index);                    // Sift up heap violating node
    void siftDown(int node_index);                  // Sifts down heap violating node
    void printKey();                                // Writes the keys in array, starting at root
    keytype peekKey();                              // Return min key without modifying the Heap
    valuetype peekValue();                          // Return min value without modifying the Heap
    keytype extractMin();                           // Removes the min key in the Heap and returns the key.
    int getParentIndex(int node_index);             // Find the parent of a given Node
    int getLeftChildIndex(int node_index);          // Find the left child of a given Node
    int getRightChildIndex(int node_index);         // Find the right of a given Node
    ~Heap();                                        // Destructor

private:
    CDA<Node<keytype, valuetype>> my_array_;        // Array of elements in min heap
    int heap_size_;                                 // Current number of elements in min heap
};


template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::Heap(keytype k[], valuetype v[], int s) {
    for (int i = 0; i < s; i++) {
        Node<keytype, valuetype> new_node(k[i], v[i]);
        my_array_.AddEnd(new_node);
    }
    heap_size_ = s;
    for (int i = ((heap_size_ / 2) - 1); i >= 0; i--) {
        siftDown(i);
    }
} 


template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::Heap() {
    CDA<Node<keytype, valuetype>> my_array_;
    heap_size_ = 0;
} 


// Copy Constructor
template <typename keytype, typename valuetype>
Heap<keytype,valuetype>::Heap(const Heap<keytype, valuetype> &heap) {
    my_array_ = heap.my_array_;
    heap_size_ = heap.heap_size_;
}


// Copy Assignment Operator
template <typename keytype, typename valuetype>
Heap<keytype, valuetype>& Heap<keytype, valuetype>::operator=(const Heap<keytype, valuetype> &heap) {
    my_array_ = heap.my_array_;
    heap_size_ = heap.heap_size_;
    return *this;
}


template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::insert(keytype k, valuetype v) {
    Node<keytype, valuetype> new_node(k, v);
    heap_size_++;
    my_array_.AddEnd(new_node);
    siftUp(heap_size_ - 1);
}


template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::siftUp(int node_index) {
    int parent_index;
    Node<keytype, valuetype> temp;
    if (node_index) {
        parent_index = getParentIndex(node_index);
        if (my_array_[parent_index] > my_array_[node_index]) {
            // Swap node and its parent ///////////
            temp = my_array_[parent_index];
            my_array_[parent_index] = my_array_[node_index];
            my_array_[node_index] = temp;
            siftUp(parent_index);
            //////////////////////////////////////
        }
    }
}


template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::siftDown(int node_index) {
    int left_child_index, right_child_index, min_index;
    Node<keytype, valuetype> temp;
    left_child_index = getLeftChildIndex(node_index);
    right_child_index = getRightChildIndex(node_index);
    if (right_child_index >= heap_size_) {
        if (left_child_index >= heap_size_) {
            return;
        }
        else {
            min_index = left_child_index;
        }
    }
    else {
        if (my_array_[left_child_index] <= my_array_[right_child_index]) {
            min_index = left_child_index;
        }
        else {
            min_index = right_child_index;
        }
    }
    if (my_array_[node_index] > my_array_[min_index]) {
        temp = my_array_[min_index];
        my_array_[min_index] = my_array_[node_index];
        my_array_[node_index] = temp;
        siftDown(min_index);
    }

}


template <typename keytype, typename valuetype>
void Heap<keytype,valuetype>::printKey() {
    for (int i = 0; i < my_array_.Length(); i++) {
        cout << my_array_[i].key << " ";
    }
    cout << endl;
}


template <typename keytype, typename valuetype>
keytype Heap<keytype,valuetype>::peekKey() {
    return my_array_[0].key;
}


template <typename keytype, typename valuetype>
valuetype Heap<keytype,valuetype>::peekValue() {
    return my_array_[0].value;
}


template <typename keytype, typename valuetype>
keytype Heap<keytype,valuetype>::extractMin() {
    keytype return_key = my_array_[0].key;
    my_array_[0] = my_array_[heap_size_ - 1];
    heap_size_--;
    if (heap_size_ > 0) {
        siftDown(0);
    }
    my_array_.DelEnd();
    return return_key;
}


template <typename keytype, typename valuetype>
int Heap<keytype,valuetype>::getParentIndex(int node_index) {
    return ((node_index - 1) / 2);
}


template <typename keytype, typename valuetype>
int Heap<keytype,valuetype>::getLeftChildIndex(int node_index) {
    return ((2 * node_index) + 1);
}


template <typename keytype, typename valuetype>
int Heap<keytype,valuetype>::getRightChildIndex(int node_index) {
    return ((2 * node_index) + 2);
}


template <typename keytype, typename valuetype>
Heap<keytype, valuetype>::~Heap() {
}


// int main(void) {

//     Heap<int,int> my_heap;
//     for (int i = 0; i < 21; i++) {
//         my_heap.insert(i,i*100);
//     }
//     my_heap.printKey();
//     cout << endl << "----------------------------------------------" << endl << endl;

//     int keys[10] = {10,9,8,7,6,5,4,3,2,1};
//     int values[10] = {100,90,80,70,60,50,40,30,20,10};
//     Heap<int,int> my_heap2(keys, values, 10);



//     cout << "Goodbye!" << endl;
// }


#endif