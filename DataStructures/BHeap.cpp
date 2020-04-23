/*
 * Implementation of a Binomial Heap.
 * 
 * This file contains two classes:
 * 1. BHeap
 * 2. BNode
 * 
 * Both of these classes are templated, with
 * two typenames: keytype and valuetype.
 * Therefore, any instantiation of BNode or
 * BHeap must be as follows:
 * 1. template <typename keytype, typename keytype>
 *    BNode<keytype, valuetype> this_name;
 * 2. template <typename keytype, typename valuetype>
 *    BHeap<keytype, valuetype> that_name;
 * 
 * 
 * @author      Stephen Gregory
 * @date        04/21/2020
 */

#ifndef BHEAP_CPP
#define BHEAP_CPP


// BNode is one element of a binomial tree inside of
// a binomial bheap (BHeap). A BNode can also be considered 
// itself a binomial tree. 
template <typename keytype, typename valuetype>
struct BNode {
    BNode() {};
    keytype key;
    valuetype value;
    int degree;
    BNode* parent; 
    BNode* sibling;
    BNode* child; 
    bool operator<(BNode const &rhs) { return key < rhs.key; };
    bool operator<=(BNode const &rhs) { return key <= rhs.key; };
    bool operator==(BNode const &rhs) { return key == rhs.key; };
    bool operator>(BNode const &rhs) { return key > rhs.key; };
    bool operator>=(BNode const &rhs) { return key >= rhs.key; };
};


// BHeap is a Binomial Min-Ordered Heap
template <typename keytype, typename valuetype>
class BHeap {
public:

    BHeap();                                                                        // Default Constructor for empty Heap.
    BHeap(keytype k[], valuetype v[], int s);                                       // Constructor using array of keys(k[]) and values([v]) with size s.
    BHeap(const BHeap &other);                                                      // Copy Constructor.
    BHeap& operator=(const BHeap& other);                                           // Copy Assignment Operator.
    BNode<keytype, valuetype>* copyHelper(BNode<keytype, valuetype>* to_copy);      // Helper function for copying a BHeap.
    void copyHelperParentAssigner(BNode<keytype, valuetype> *bnode);                // Assign correct parents to all of the nodes in the heap.

    keytype peekKey();                                                              // Return min key without modifying the Heap.
    valuetype peekValue();                                                          // Return min value without modifying the Heap.
    keytype extractMin();                                                           // Removes the min key in the Heap and returns the key.
    BNode<keytype, valuetype>* reverseChildren(BNode<keytype, valuetype>* child_ptr);   // Reverses the order of children, given the leftmost child.
    void insert(keytype k, valuetype v);                                            // Inserts a node with key k and value v into the heap.
    void merge(BHeap<keytype, valuetype> &bheap2);                                  // Merges the Heap H2 into the current heap.
    void mergeEqualDegree();                                                        // Merge all trees of equal degree.
    void printKey();                                                                // Prints the keys in the heap.
    void printBinomialTree(BNode<keytype, valuetype>* root);                        // Prints the keys of one specific binomial tree.
    void setHead(BNode<keytype, valuetype>* head);                                  // Setter for the head.
    BNode<keytype, valuetype>* getHead();                                           // Getter for the head.

    ~BHeap();                                                                       // Destructor.
    void destroy(BNode<keytype, valuetype>* bnode);                                 // Helper function for destructor.


private:

    BNode<keytype, valuetype>* head;                                                // Pointer to leftmost Binomial Tree root.
    void initializeBNode(BNode<keytype, valuetype>* bnode, keytype key, valuetype value, int degree=0);
                                                                                    // Initializes bnode with default values,
                                                                                    // all pointers point to nullptr.
    // Merge two binomial trees of the same degree
    static void linkBinomialTrees(BNode<keytype, valuetype>* x, BNode<keytype, valuetype>* y);
                                                                                    // Merge two Binomial Trees of the same degree.
};


template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap() {
    head = nullptr;
}


template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::BHeap(keytype k[], valuetype v[], int s) {
    head = nullptr;
    for (int i = 0; i < s; i++) {
        insert(k[i], v[i]);
    }
}


// Copy Assignment Operator
template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>& BHeap<keytype, valuetype>::operator=(const BHeap<keytype, valuetype>& other) {
    if (head == head) {
        return *this;
    }
    delete head;
    head = nullptr;
    BHeap<keytype, valuetype> temp(other);
    head = temp.head;
    temp.head = nullptr;
    return *this;
}


// Copy Constructor
template <typename keytype, typename valuetype>
BHeap<keytype,valuetype>::BHeap(const BHeap<keytype, valuetype> &bheap) {
    head = copyHelper(bheap.head);
    BNode<keytype, valuetype>* curr = head;
    copyHelperParentAssigner(curr);
}


template <typename keytype, typename valuetype>
BNode<keytype, valuetype>* BHeap<keytype,valuetype>::copyHelper(BNode<keytype, valuetype>* to_copy) {
    if (to_copy == nullptr) {
        return nullptr;
    }
    BNode<keytype, valuetype>* copy_node = new BNode<keytype, valuetype>;
    copy_node->key = to_copy->key;
    copy_node->value = to_copy->value;
    copy_node->degree = to_copy->degree;
    copy_node->parent = nullptr;
    copy_node->child = copyHelper(to_copy->child);
    copy_node->sibling = copyHelper(to_copy->sibling);
    return copy_node;
}


template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::copyHelperParentAssigner(BNode<keytype, valuetype>* bnode) {
    if (bnode == nullptr) {
        return;
    }
    BNode<keytype, valuetype>* child_ptr = bnode->child;
    while(child_ptr != nullptr) {
        child_ptr->parent = bnode;
        child_ptr = child_ptr->sibling;
    }
    copyHelperParentAssigner(bnode->sibling);
    copyHelperParentAssigner(bnode->child);
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::initializeBNode(BNode<keytype, valuetype>* bnode, keytype key, valuetype value, int degree) {
    bnode->key = key;
    bnode->value = value;
    bnode->degree = degree;
    bnode->parent = nullptr;
    bnode->child = nullptr;
    bnode->sibling = nullptr;
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::linkBinomialTrees(BNode<keytype, valuetype>* x, BNode<keytype, valuetype>* y) {
    // make x the parent of y
    y->parent = x;
    y->sibling = x->child;
    x->child = y;

    // increase the degree of x
    x->degree += 1;
}


template <typename keytype, typename valuetype>
keytype BHeap<keytype,valuetype>::peekKey() {
    BNode<keytype, valuetype>* curr = head;
    BNode<keytype, valuetype>* prev_min = nullptr;
    // Find the root with minimum key
    BNode<keytype, valuetype>* min_ptr = nullptr;
    BNode<keytype, valuetype>* prev_ptr = nullptr;
    keytype min;
    if (curr != nullptr) {
        min = curr->key;
    }
    while (curr != nullptr) {
        if (curr->key <= min) {
            min = curr->key;
            prev_min = prev_ptr;
            min_ptr = curr;
        }
        prev_ptr = curr;
        curr = curr->sibling;
    }
    return min_ptr->key;
}


template <typename keytype, typename valuetype>
valuetype BHeap<keytype,valuetype>::peekValue() {
    BNode<keytype, valuetype>* curr = head;
    BNode<keytype, valuetype>* prev_min = nullptr;
    // Find the root with minimum key
    BNode<keytype, valuetype>* min_ptr = nullptr;
    BNode<keytype, valuetype>* prev_ptr = nullptr;
    keytype min;
    if (curr != nullptr) {
        min = curr->key;
    }
    while (curr != nullptr) {
        if (curr->key <= min) {
            min = curr->key;
            prev_min = prev_ptr;
            min_ptr = curr;
        }
        prev_ptr = curr;
        curr = curr->sibling;
    }
    return min_ptr->value;
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::insert(keytype key, valuetype value) {
    if (head == nullptr) {
        BNode<keytype, valuetype>* first_node = new BNode<keytype, valuetype>;
        initializeBNode(first_node, key, value, 0);
        head = first_node;
        return;
    }

    BHeap<keytype, valuetype> new_bheap;
    BNode<keytype, valuetype>* new_bnode = new BNode<keytype, valuetype>;
    initializeBNode(new_bnode, key, value, 0);
    new_bheap.setHead(new_bnode);
    merge(new_bheap);
    new_bheap.head = nullptr;
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::setHead(BNode<keytype, valuetype>* head) {
    this->head = head;
}


template <typename keytype, typename valuetype>
BNode<keytype, valuetype>* BHeap<keytype, valuetype>::getHead() {
    return head;
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::merge(BHeap<keytype, valuetype> &bheap2) {
    BNode<keytype, valuetype>* curr1 = getHead();
    BNode<keytype, valuetype>* curr2 = bheap2.getHead();
    BNode<keytype, valuetype>* curr3 = nullptr;
    BNode<keytype, valuetype>* temp = nullptr;

    if (curr1 == nullptr) {
        setHead(curr2);
        bheap2.head = nullptr;
        return;
    }

    if (curr1->degree <= curr2->degree) {
        curr3 = curr1;
        curr1 = curr1->sibling;
    }
    else {
        curr3 = curr2;
        curr2 = curr2->sibling;
    }

    temp = curr3;

    // Merge the two heaps WITHOUT taking care of trees
    // that have the same degree.
    // The roots of the trees must be in non-decreasing order 
    // from left to right
    while (curr1 != nullptr && curr2 != nullptr) {
        if (curr1->degree <= curr2->degree) {
            curr3->sibling = curr1;
            curr1 = curr1->sibling;
        }
        else {
            curr3->sibling = curr2;
            curr2 = curr2->sibling;
        }
        curr3 = curr3->sibling;
    }

    if (curr1 != nullptr) {
        // copy all the remaining trees in bheap1
        while (curr1 != nullptr) {
            curr3->sibling = curr1;
            curr1 = curr1->sibling;
            curr3 = curr3->sibling;
        }
    }

    if (curr2 != nullptr) {
        // copy all the remaining trees in bheap2
        while (curr2 != nullptr) {
            curr3->sibling = curr2;
            curr2 = curr2->sibling;
            curr3 = curr3->sibling;
        }
    }

    // Go through the merged list of roots and merge all 
    // trees of equal degree.

    setHead(temp);
    mergeEqualDegree();
    bheap2.head = nullptr;
}

template <typename keytype, typename valuetype>
void BHeap<keytype,valuetype>::mergeEqualDegree() {
    BNode<keytype, valuetype>* curr = head;
    BNode<keytype, valuetype>* prev = nullptr;
    BNode<keytype, valuetype>* next = curr->sibling;

    while (next != nullptr) {
        // if two adjacent roots have different degree OR 3 consecutive
        // roots have the same degree (watching against the end of the list), move to the next tree.
        if ((curr->degree != next->degree) || (next->sibling != nullptr && (curr->degree == next->sibling->degree))) {
            prev = curr;
            curr = next;
        }
        // otherwise, merge binomial trees with the same degree
        else {
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                BHeap::linkBinomialTrees(curr, next);
            }
            else {
                if (prev == nullptr) {
                    this->head = next;
                }
                else {
                    prev->sibling = next;
                }
                BHeap::linkBinomialTrees(next, curr);
                curr = next;
            }
        }
        next = curr->sibling;
    }
}


template <typename keytype, typename valuetype>
keytype BHeap<keytype, valuetype>::extractMin() {
    BNode<keytype, valuetype>* curr = head;
    BNode<keytype, valuetype>* prev_min = nullptr;
    // Find the root with minimum key
    BNode<keytype, valuetype>* min_ptr = nullptr;
    BNode<keytype, valuetype>* prev_ptr = nullptr;
    keytype min = curr->key;

    while (curr != nullptr) {
        if (curr->key <= min) {
            min = curr->key;
            prev_min = prev_ptr;
            min_ptr = curr;
        }
        prev_ptr = curr;
        curr = curr->sibling;
    }

    // Delete the node pointed to by min_ptr
    if (prev_min != nullptr && min_ptr->sibling != nullptr) {
        prev_min->sibling = min_ptr->sibling;
    }
    else if (prev_min != nullptr && min_ptr->sibling == nullptr) {
        prev_min->sibling = nullptr;
    }
    else {
        setHead(min_ptr->sibling);
    }

    // If there are no children, just delete the node and return the min key
    if (min_ptr->child == nullptr) {
        delete min_ptr;
        min_ptr = nullptr;
        return min;
    }

    // Remove the parent reference from all of the children
    BNode<keytype, valuetype>* child_ptr = min_ptr->child;
    while (child_ptr != nullptr) {
        child_ptr->parent = nullptr;
        child_ptr = child_ptr->sibling;
    }

    // Reverse the order of the children
    BNode<keytype, valuetype>* child_list_head = reverseChildren(min_ptr->child);

    // Create a new child_heap with the child subtrees
    BHeap<keytype, valuetype> child_heap;
    child_heap.setHead(child_list_head);

    // Merge the children heap and the original heap
    merge(child_heap);
    // child_heap.head = nullptr;
    delete min_ptr;
    min_ptr = nullptr;
    return min;
}


template <typename keytype, typename valuetype>
BNode<keytype, valuetype>* BHeap<keytype, valuetype>::reverseChildren(BNode<keytype, valuetype>* child_ptr) {
    BNode<keytype, valuetype>* prev = nullptr;
    BNode<keytype, valuetype>* curr = child_ptr;
    while (curr != nullptr) {
        // Store the sibling
        BNode<keytype, valuetype>* next = curr->sibling;
        // Reverse current node's sibling with previous
        curr->sibling = prev;
        // Move both pointers one position ahead
        prev = curr;
        curr = next;
    }
    return prev;
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::printKey() {
    std::cout << endl;
    BNode<keytype, valuetype>* curr_root = head;
    if (curr_root == nullptr) {
        std::cout << "Empty Binomial Heap." << endl;
        return;
    }

    while (curr_root != nullptr) {
        std::cout << "B" << curr_root->degree << endl;
        std::cout << curr_root->key << " ";
        printBinomialTree(curr_root->child);
        curr_root = curr_root->sibling;
        std::cout << endl << endl;
    }
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::printBinomialTree(BNode<keytype, valuetype>* root) {
    if (root == nullptr) {
        return;
    }
    std::cout << root->key << " ";
    // First, print the subtree at the leftmost child
    printBinomialTree(root->child);
    // Then, print the subtree at the next sibling
    printBinomialTree(root->sibling);
}


template <typename keytype, typename valuetype>
BHeap<keytype, valuetype>::~BHeap() {
    // cout << "~~~In Destructor~~~\n";
    destroy(head);
    // cout << "~~~Leaving Destructor~~~\n";
}


template <typename keytype, typename valuetype>
void BHeap<keytype, valuetype>::destroy(BNode<keytype, valuetype>* bnode){
    if (bnode != nullptr) {
        destroy(bnode->child);
        bnode->child = nullptr;
        destroy(bnode->sibling);
        bnode->sibling = nullptr;
        // cout << "Deleting bnode with key = " << bnode->key << endl;
        delete bnode;
    }
}


#endif