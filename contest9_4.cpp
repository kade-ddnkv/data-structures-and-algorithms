// C++ program for B-Tree insertion
#include <iostream>

// A BTree node
class BTreeNode {
    int *keys_;      // An array of keys
    int t_;          // Minimum degree (defines the range for number of keys)
    BTreeNode **c_;  // An array of child pointers
    int n_;          // Current number of keys
    bool leaf_;      // Is true when node is leaf. Otherwise false

public:
    BTreeNode(int, bool);  // Constructor

    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(int);

    // A utility function to split the child y of this node. i is index of y in
    // child array C[].  The Child y must be full when this function is called
    void splitChild(int, BTreeNode *);

    int64_t sum() {
        int64_t sum = 0;
        for (int64_t i = 0; i < n_; ++i) {
            sum += keys_[i];
        }
        return sum;
    }

    ~BTreeNode() {
        delete[] keys_;
        if (!leaf_) {
            for (int64_t i = 0; i < n_ + 1; ++i) {
                delete c_[i];
            }
        }
        delete[] c_;
    }

    // Make BTree friend of this so that we can access private members of this
    // class in BTree functions
    friend class BTree;
};

// A BTree
class BTree {
    BTreeNode *root_;  // Pointer to root node
    int t_;            // Minimum degree

public:
    // Constructor (Initializes tree as empty)
    explicit BTree(int degree) {
        root_ = nullptr;
        t_ = degree;
    }

    // The main function that inserts a new key in this B-Tree
    void insert(int key);

    BTreeNode *findRecursive(BTreeNode *curr, int64_t key_to_find) {
        if (curr->leaf_) {
            for (int64_t i = 0; i < curr->n_; ++i) {
                if (curr->keys_[i] == key_to_find) {
                    return curr;
                }
            }
            return nullptr;
        }

        for (int64_t i = 0; i < curr->n_; ++i) {
            if (key_to_find < curr->keys_[i]) {
                return findRecursive(curr->c_[i], key_to_find);
            } else if (key_to_find == curr->keys_[i]) {
                return curr;
            }
        }
        return findRecursive(curr->c_[curr->n_], key_to_find);
    }

    bool containsKey(int64_t key) {
        if (root_ == nullptr) {
            return false;
        }
        return findRecursive(root_, key) != nullptr;
    }

    int64_t getSizeRecursive(BTreeNode *curr) const {
        if (curr->leaf_) {
            return 1;
        }
        int64_t nodes_count = 1;
        for (int64_t i = 0; i < curr->n_ + 1; ++i) {
            nodes_count += getSizeRecursive(curr->c_[i]);
        }
        return nodes_count;
    }

    int64_t getSumRecursive(BTreeNode *curr) const {
        if (curr->leaf_) {
            return curr->sum();
        }
        int64_t curr_sum = curr->sum();
        for (int64_t i = 0; i < curr->n_ + 1; ++i) {
            curr_sum += getSumRecursive(curr->c_[i]);
        }
        return curr_sum;
    }

    size_t size() const {
        if (root_ == nullptr) {
            return 0;
        }
        return getSizeRecursive(root_);
    }

    int64_t sum() const {
        if (root_ == nullptr) {
            return 0;
        }
        return getSumRecursive(root_);
    }

    ~BTree() {
        delete root_;
    }
};

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int degree, bool leaf) {
    // Copy the given minimum degree and leaf property
    t_ = degree;
    leaf_ = leaf;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys_ = new int[2 * t_ - 1];
    c_ = new BTreeNode *[2 * t_];

    // Initialize the number of keys as 0
    n_ = 0;
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(int key) {
    if (containsKey(key)) {
        return;
    }
    // If tree is empty
    if (root_ == nullptr) {
        // Allocate memory for root
        root_ = new BTreeNode(t_, true);
        root_->keys_[0] = key;  // Insert key
        root_->n_ = 1;          // Update number of keys in root
    } else {
        // If root is full, then tree grows in height
        if (root_->n_ == 2 * t_ - 1) {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t_, false);

            // Make old root as child of new root
            s->c_[0] = root_;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root_);

            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int index = 0;
            if (s->keys_[0] < key) {
                index++;
            }
            s->c_[index]->insertNonFull(key);

            // Change root
            root_ = s;
        } else {  // If root is not full, call insertNonFull for root
            root_->insertNonFull(key);
        }
    }
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(int key) {
    // Initialize index as index of rightmost element
    int index = n_ - 1;

    // If this is a leaf node
    if (leaf_) {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (index >= 0 && keys_[index] > key) {
            keys_[index + 1] = keys_[index];
            index--;
        }

        // Insert the new key at found location
        keys_[index + 1] = key;
        n_ = n_ + 1;
    } else {
        // Find the child which is going to have the new key
        while (index >= 0 && keys_[index] > key) {
            index--;
        }

        // See if the found child is full
        if (c_[index + 1]->n_ == 2 * t_ - 1) {
            // If the child is full, then split it
            splitChild(index + 1, c_[index + 1]);

            // After split, the middle key of C[index] goes up and
            // C[index] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys_[index + 1] < key) {
                index++;
            }
        }
        c_[index + 1]->insertNonFull(key);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int index, BTreeNode *y) {
    // Create a new node which is going to store (t-1) keys
    // of y
    BTreeNode *z = new BTreeNode(y->t_, y->leaf_);
    z->n_ = t_ - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t_ - 1; ++j) {
        z->keys_[j] = y->keys_[j + t_];
    }

    // Copy the last t children of y to z
    if (!y->leaf_) {
        for (int j = 0; j < t_; ++j) {
            z->c_[j] = y->c_[j + t_];
        }
    }

    // Reduce the number of keys in y
    y->n_ = t_ - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n_; j >= index + 1; --j) {
        c_[j + 1] = c_[j];
    }

    // Link the new child to this node
    c_[index + 1] = z;

    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n_ - 1; j >= index; --j) {
        keys_[j + 1] = keys_[j];
    }

    // Copy the middle key of y to this node
    keys_[index] = y->keys_[t_ - 1];

    // Increment count of keys in this node
    n_ = n_ + 1;
}
