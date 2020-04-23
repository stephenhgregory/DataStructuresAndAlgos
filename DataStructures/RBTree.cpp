#include <iostream>

using namespace std;

enum Color {BLACK, RED};

template <typename keytype, typename valuetype>
struct Node {
    keytype key_;
    valuetype value_;
    Color color_;
    Node* left_child_;
    Node* right_child_;
    Node* parent_;
    bool null_flag_;
    int subtree_size_;
    Node() {
        null_flag_ = false;
    }
};


template <typename keytype, typename valuetype>
class RBTree {
    public:
        RBTree();
        RBTree(keytype k[], valuetype V[], int s);
        RBTree(const RBTree<keytype, valuetype> &old_tree) {
            TNULL_ = new Node<keytype, valuetype>;
            TNULL_->color_ = BLACK;
            TNULL_->null_flag_ = true;
            TNULL_->subtree_size_ = old_tree.TNULL_->subtree_size_;
            size_ = old_tree.size_;
            root_ = copyTree(old_tree.root_, TNULL_);
        }

        Node<keytype, valuetype>* copyTree(Node<keytype, valuetype>* old_root, Node<keytype, valuetype>* tempparent) {
            if (old_root->null_flag_) {
                return 0;
            }
            Node<keytype, valuetype>* p = new Node<keytype, valuetype>;
            p->key_ = old_root->key_;
            p->value_ = old_root->value_;
            p->color_ = old_root->color_;
            p->subtree_size_ = old_root->subtree_size_;
            p->parent_ = tempparent;

            if (!old_root->left_child_->null_flag_) {
                p->left_child_ = copyTree(old_root->left_child_, p);
            }
            else {
                p->left_child_ = TNULL_;
            }
            if (!old_root->right_child_->null_flag_) {
                p->right_child_ = copyTree(old_root->right_child_, p);
            }
            else {
                p->right_child_ = TNULL_;
            }
            return p;
        }

        RBTree<keytype, valuetype>& operator=(RBTree<keytype, valuetype> other) {
            size_ = other.size_;
            swap(*this, other);

            return *this;
        }

        void swap(RBTree<keytype, valuetype>& first, RBTree<keytype, valuetype>& second) {
            using std::swap;

            swap(first.root_, second.root_);
            swap(first.TNULL_, second.TNULL_);
            swap(first.size_, second.size_);
        }

        void initializeRoot(Node<keytype, valuetype>*);

        void leftRotate(Node<keytype, valuetype>*);
        void rightRotate(Node<keytype, valuetype>*);
        void RBInsertFixup(Node<keytype, valuetype>*);
        void RBInsert(Node<keytype, valuetype>*);
        void insert(keytype k, valuetype v);

        int remove(keytype k);
        void decrementSubtreeSize(Node<keytype, valuetype>*);
        void RBTransplant(Node<keytype, valuetype>*, Node<keytype, valuetype>*);
        void RBDeleteFixup(Node<keytype, valuetype>*);

        valuetype* search(keytype k);
        Node<keytype, valuetype>* searchNode(keytype k);

        Node<keytype, valuetype>* treeMinimum(Node<keytype, valuetype>*);
        Node<keytype, valuetype>* treeMaximum(Node<keytype, valuetype>*);
        keytype* successor(keytype k);
        keytype* predecessor(keytype k);

        void preorder();
        void printPreorder(Node<keytype, valuetype>*);
        void inorder();
        void printInorder(Node<keytype, valuetype>*);
        void postorder();
        void printPostorder(Node<keytype, valuetype>*);

        int size();
        int rank(keytype k);
        keytype select(int);
        keytype OSSelect(Node<keytype, valuetype>* x, int);

        ~RBTree() {
            deleteTree(root_);
        }

        void deleteTree(Node<keytype, valuetype>* root) {
            if (root != TNULL_) {
                deleteTree(root->left_child_);
                deleteTree(root->right_child_);
                delete root;
            }
            delete TNULL_;
        }


    private:
        int size_;
        Node<keytype, valuetype>* root_;
        Node<keytype, valuetype>* TNULL_;
};


template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree() {
    TNULL_ = new Node<keytype, valuetype>;
    root_ = new Node<keytype, valuetype>;
    TNULL_->color_ = BLACK;
    TNULL_->subtree_size_ = 0;
    TNULL_->null_flag_ = true;
    root_ = TNULL_;
    size_ = 0;
}


template <typename keytype, typename valuetype>
RBTree<keytype, valuetype>::RBTree(keytype k[], valuetype v[], int s) {
    TNULL_ = new Node<keytype, valuetype>;
    root_ = new Node<keytype, valuetype>;
    TNULL_->color_ = BLACK;
    TNULL_->subtree_size_ = 0;
    TNULL_->null_flag_ = true;
    root_ = TNULL_;
    size_ = 0;

    for (int i = 0; i < s; i++) {
        insert(k[i], v[i]);
    }
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::initializeRoot(Node<keytype, valuetype>* z) {
    root_ = z;
    root_->subtree_size_ = 1;
    root_->left_child_ = TNULL_;
    root_->right_child_ = TNULL_;
    TNULL_->left_child_ = root_;
    TNULL_->right_child_ = root_;
    root_->parent_ = TNULL_;
    root_->color_ = BLACK;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::leftRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = x->right_child_; // set y
    x->right_child_ = y->left_child_;              // Set x's right subtree to be y's left subtree

    if (y->left_child_ != TNULL_) {
        y->left_child_->parent_ = x;
    }
    y->parent_ = x->parent_;                       // Link x's parent to y
    if (x->parent_ == TNULL_) {
        root_ = y;
    }
    else if (x == x->parent_->left_child_) {
        x->parent_->left_child_ = y;
    }
    else {
        x->parent_->right_child_ = y;
    }
    y->left_child_ = x;                            // put x on y's left
    x->parent_ = y;

    // Update subtree_size_ for x and y
    y->subtree_size_ = x->subtree_size_;
    x->subtree_size_ = x->left_child_->subtree_size_ + x->right_child_->subtree_size_ + 1;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::rightRotate(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* y = x->left_child_;  // set y
    x->left_child_ = y->right_child_;              // Set x's left subtree to be y's right subtree

    if (y->right_child_ != TNULL_) {
        y->right_child_->parent_ = x;
    }
    y->parent_ = x->parent_;                       // Link x's parent to y
    if (x->parent_ == TNULL_) {
        root_ = y;
    }
    else if (x == x->parent_->right_child_) {
        x->parent_->right_child_ = y;
    }
    else {
        x->parent_->left_child_ = y;
    }
    y->right_child_ = x;                            // put x on y's left
    x->parent_ = y;

    // Update subtree_size_ for x and y
    y->subtree_size_ = x->subtree_size_;
    x->subtree_size_ = x->left_child_->subtree_size_ + x->right_child_->subtree_size_ + 1;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::RBInsertFixup(Node<keytype, valuetype>* z) {
    while (z->parent_->color_ == RED) {                                         // while z's parent is red
        if (z->parent_ == z->parent_->parent_->left_child_) {                   // if z's parent is a left child
            Node<keytype, valuetype>* y = z->parent_->parent_->right_child_;    // set y as z's uncle
            if (y->color_ == RED) {                                             // if z's uncle (y) is red
                z->parent_->color_ = BLACK;                                     // CASE 1
                y->color_ = BLACK;                                              // CASE 1
                z->parent_->parent_->color_ = RED;                              // CASE 1
                z = z->parent_->parent_;                                        // CASE 1
                continue;
            }
            else if (z == z->parent_->right_child_) {                           // else if z is a right child
                z = z->parent_;                                                 // CASE 2
                leftRotate(z);                                                  // CASE 2
            }
            z->parent_->color_ = BLACK;                                         // CASE 3
            z->parent_->parent_->color_ = RED;                                  // CASE 3
            rightRotate(z->parent_->parent_);                                   // CASE 3
        }
        else {                                                                  // else if z's parent is a right child
            Node<keytype, valuetype>* y = z->parent_->parent_->left_child_;     // set y as z's uncle
            if (y->color_ == RED) {                                             // if z's uncle is red
                z->parent_->color_ = BLACK;                                     // CASE 1
                y->color_ = BLACK;                                              // CASE 1
                z->parent_->parent_->color_ = RED;                              // CASE 1
                z = z->parent_->parent_;                                        // CASE 1
                continue;
            }
            else if (z == z->parent_->left_child_) {                            // else if z is a left child
                z = z->parent_;                                                 // CASE 2
                rightRotate(z);                                                 // CASE 2
            }
                z->parent_->color_ = BLACK;                                         // CASE 3
                z->parent_->parent_->color_ = RED;                                  // CASE 3
                leftRotate(z->parent_->parent_);                                    // CASE 3
        }
    }
    root_->color_ = BLACK;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::RBInsert(Node<keytype, valuetype>* z) {

    if (root_ == NULL) {
        initializeRoot(z);
        return;
    }

    Node<keytype, valuetype>* y = TNULL_;
    Node<keytype, valuetype>* x = root_;
    while (x != TNULL_) {
        y = x;
        if (z->key_ < x->key_) {
            x->subtree_size_++;
            x = x->left_child_;
        }
        else {
            x->subtree_size_++;
            x = x->right_child_;
        }
        z->parent_ = y;
    }

    if (y == TNULL_) {
        root_ = z;
        root_->parent_ = TNULL_;
    }

    if (z->key_ < y->key_) {
        y->left_child_ = z;
    }
    else {
        y->right_child_ = z;
    }

    z->left_child_ = TNULL_;
    z->right_child_ = TNULL_;
    z->color_ = RED;
    RBInsertFixup(z);
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::insert(keytype k, valuetype v) {
    size_++;
    Node<keytype, valuetype>* new_node = new Node<keytype, valuetype>;
    new_node->key_ = k;
    new_node->value_ = v;
    new_node->subtree_size_ = 1;
    RBInsert(new_node);
    TNULL_->parent_ = root_;
}


template <typename keytype, typename valuetype>
valuetype* RBTree<keytype, valuetype>::search(keytype k) { // iterative binary search
    Node<keytype, valuetype>* x = root_;
    while ((x != TNULL_) && (k != x->key_)) {
        if (k < x->key_) {
            x = x->left_child_;
        }
        else {
            x = x->right_child_;
        }
    }
    if (x == TNULL_) {
        return nullptr;
    }
    return &x->value_;
}


template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::searchNode(keytype k) { // iterative binary search
    Node<keytype, valuetype>* x = root_;
    while ((x != TNULL_) && (k != x->key_)) {
        if (k < x->key_) {
            x = x->left_child_;
        }
        else {
            x = x->right_child_;
        }
    }
    if (x == TNULL_) {
        return nullptr;
    }
    return x;
}


template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::size() { 
    return size_;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::preorder() {
    printPreorder(root_);
    cout << endl;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPreorder(Node<keytype, valuetype>* x) {
    if (x == TNULL_) {
        return;
    }
    cout << x->key_ << " ";
    printPreorder(x->left_child_);
    printPreorder(x->right_child_);
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::inorder() {
    printInorder(root_);
    cout << endl;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printInorder(Node<keytype, valuetype>* x) {
    if (x == TNULL_) {
        return;
    }
    printInorder(x->left_child_);
    cout << x->key_ << " ";
    printInorder(x->right_child_);
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::postorder() {
    printPostorder(root_);
    cout << endl;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::printPostorder(Node<keytype, valuetype>* x) {
    if (x == TNULL_) {
        return;
    }
    printPostorder(x->left_child_);
    printPostorder(x->right_child_);
    cout << x->key_ << " ";
}


template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::treeMinimum(Node<keytype, valuetype>* x) {
    while (x->left_child_ != TNULL_) {
        x = x->left_child_;
    }
    return x;
}


template <typename keytype, typename valuetype>
Node<keytype, valuetype>* RBTree<keytype, valuetype>::treeMaximum(Node<keytype, valuetype>* x) {
    while (x->right_child_ != TNULL_) {
        x = x->right_child_;
    }
    return x;
}


template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::successor(keytype k) {
    Node<keytype, valuetype>* x = searchNode(k);
    if (x->right_child_ != TNULL_) {
        return &treeMinimum(x->right_child_)->key_;
    }
    Node<keytype, valuetype>* y = x->parent_;
    while ((y != TNULL_) && (x == y->right_child_)) {
        x = y;
        y = y->parent_;
    }
    return &y->key_;
}


template <typename keytype, typename valuetype>
keytype* RBTree<keytype, valuetype>::predecessor(keytype k) {
    Node<keytype, valuetype>* x = searchNode(k);
    if (x->left_child_ != TNULL_) {
        return &treeMaximum(x->right_child_)->key_;
    }
    Node<keytype, valuetype>* y = x->parent_;
    while ((y != TNULL_) && (x == y->left_child_)) {
        x = y;
        y = y->parent_;
    }
    return &y->key_;
}

template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::RBTransplant(Node<keytype, valuetype>* u, Node<keytype, valuetype>* v) {
    if (u->parent_ == TNULL_) {
        root_ = v;
    }
    else if (u == u->parent_->left_child_) {
        u->parent_->left_child_ = v;
    }
    else {
        u->parent_->right_child_ = v;
    }
    v->parent_ = u->parent_;
}


template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::remove(keytype k) {
    Node<keytype, valuetype>* z = searchNode(k);
    if (z == NULL) {
        return 0;
    }
    Node<keytype, valuetype>* y = z;
    Node<keytype, valuetype>* x = nullptr;
    Color y_original_color = y->color_;
    if (z->left_child_ == TNULL_) {
        x = z->right_child_;
        RBTransplant(z, z->right_child_);
        decrementSubtreeSize(z);
    }
    else if (z->right_child_ == TNULL_) {
        x = z->left_child_;
        RBTransplant(z, z->left_child_);
        decrementSubtreeSize(z);
    }
    else {
        y = treeMinimum(z->right_child_);
        decrementSubtreeSize(y);
        y_original_color = y->color_;
        x = y->right_child_;

        if (y->parent_ == z) {
            x->parent_ = y;
        }
        else {
            RBTransplant(y, y->right_child_);
            y->right_child_ = z->right_child_;
            y->right_child_->parent_ = y;
        }
        RBTransplant(z, y);
        y->left_child_ = z->left_child_;
        y->left_child_->parent_ = y;
        y->subtree_size_ = y->left_child_->subtree_size_ + y->right_child_->subtree_size_ + 1;
        y->color_ = z->color_;
    }
    if (y_original_color == BLACK) {
        RBDeleteFixup(x);
    }
    size_--;
    return 1;
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::decrementSubtreeSize(Node<keytype, valuetype>* x) {
    while (x != TNULL_) {
        x->subtree_size_--;
        x = x->parent_;
    }
}


template <typename keytype, typename valuetype>
void RBTree<keytype, valuetype>::RBDeleteFixup(Node<keytype, valuetype>* x) {
    Node<keytype, valuetype>* w = nullptr;
    while ((x != root_) && (x->color_ == BLACK)) {
        if (x == x->parent_->left_child_) {
            w = x->parent_->right_child_;
            if (w->color_ == RED) {
                w->color_ = BLACK;                              // CASE 1
                x->parent_->color_ = RED;                       // CASE 1
                leftRotate(x->parent_);                         // CASE 1
                w = x->parent_->right_child_;                   // CASE 1
            }
            if ((w->left_child_->color_ == BLACK) && (w->right_child_->color_ == BLACK)) {
                w->color_ = RED;                                // CASE 2
                x = x->parent_;                                 // CASE 2
                continue;
            }
            else if (w->right_child_->color_ == BLACK) {
                w->left_child_->color_ = BLACK;                 // CASE 3
                w->color_ = RED;                                // CASE 3
                rightRotate(w);                                 // CASE 3
                w = x->parent_->right_child_;                   // CASE 3
            }             
            // if ((w->color_ == BLACK) && (w->right_child_->color_ == RED)) {
                w->color_ = x->parent_->color_;                 // CASE 4
                x->parent_->color_ = BLACK;                     // CASE 4
                w->right_child_->color_ = BLACK;                // CASE 4
                leftRotate(x->parent_);                         // CASE 4
                x = root_;                                      // CASE 4
            // }
        }

        else {
            w = x->parent_->left_child_;
            if (w->color_ == RED) {
                w->color_ = BLACK;                              // CASE 1
                x->parent_->color_ = RED;                       // CASE 1
                rightRotate(x->parent_);                        // CASE 1
                w = x->parent_->left_child_;                    // CASE 1

            }
            if ((w->right_child_->color_ == BLACK) && (w->left_child_->color_ == BLACK)) {
                w->color_ = RED;                                // CASE 2
                x = x->parent_;                                 // CASE 2
                continue;
            }
            else if (w->left_child_->color_ == BLACK) {
                w->right_child_->color_ = BLACK;                // CASE 3
                w->color_ = RED;                                // CASE 3
                leftRotate(w);                                  // CASE 3
                w = x->parent_->left_child_;                    // CASE 3
            }             
            // if ((w->color_ == BLACK) && (w->left_child_->color_ == RED)) {
                w->color_ = x->parent_->color_;                 // CASE 4
                x->parent_->color_ = BLACK;                     // CASE 4
                w->left_child_->color_ = BLACK;                 // CASE 4
                rightRotate(x->parent_);                        // CASE 4
                x = root_;                                      // CASE 4
            // }
        }
    }
    x->color_ = BLACK;
}


template <typename keytype, typename valuetype>
int RBTree<keytype, valuetype>::rank(keytype k) {
    Node<keytype, valuetype>* x = searchNode(k);
    Node<keytype, valuetype>* y = x;
    int r = x->left_child_->subtree_size_ + 1;
    while (y != root_) {
        if (y == y->parent_->right_child_) {
            r = r + y->parent_->left_child_->subtree_size_ + 1;
        }
        y = y->parent_;
    }
    return r;
}


template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::select(int pos) {
    return OSSelect(root_, pos);
}


template <typename keytype, typename valuetype>
keytype RBTree<keytype, valuetype>::OSSelect(Node<keytype, valuetype>* x, int pos) {

    int r = x->left_child_->subtree_size_ + 1;
    if (pos == r) {
        return x->key_;
    }
    else if (pos < r) {
        return OSSelect(x->left_child_, pos);
    }
    else {
        return OSSelect(x->right_child_, pos - r);
    }
}
