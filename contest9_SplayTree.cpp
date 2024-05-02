#include <iostream>

struct Node {
    friend class SplayTree;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

public:
    int key;

    explicit Node(int key) {
        this->key = key;
    }

    Node(int key, Node *parent) {
        this->key = key;
        this->parent = parent;
    }

    ~Node() {
        delete left;
        delete right;
    }
};

class SplayTree {
    Node *root_ = nullptr;

    int myMax(int first, int second) const {
        return first > second ? first : second;
    }

    int getHeightRecursive(Node *root) const {
        if (root == nullptr) {
            return 0;
        }
        return myMax(getHeightRecursive(root->left), getHeightRecursive(root->right)) + 1;
    }

    void rightRotate(Node *pivot) {
        // Поворот относительно вершины направо.
        Node *next_left = pivot->left->right;

        if (pivot->parent && pivot->parent->left == pivot) {
            pivot->parent->left = pivot->left;
        } else if (pivot->parent && pivot->parent->right == pivot) {
            pivot->parent->right = pivot->left;
        }
        pivot->left->parent = pivot->parent;

        pivot->parent = pivot->left;
        pivot->parent->right = pivot;

        pivot->left = next_left;
        if (next_left) {
            next_left->parent = pivot;
        }
    }

    void leftRotate(Node *pivot) {
        // Поворот относительно вершины налево.
        Node *next_right = pivot->right->left;

        if (pivot->parent && pivot->parent->left == pivot) {
            pivot->parent->left = pivot->right;
        } else if (pivot->parent && pivot->parent->right == pivot) {
            pivot->parent->right = pivot->right;
        }
        pivot->right->parent = pivot->parent;

        pivot->parent = pivot->right;
        pivot->parent->left = pivot;

        pivot->right = next_right;
        if (next_right) {
            next_right->parent = pivot;
        }
    }

public:
    SplayTree() {
    }

    ~SplayTree() {
        delete root_;
    }

    void insert(int key) {
        if (root_ == nullptr) {
            root_ = new Node(key);
            return;
        }

        Node *curr = root_;
        bool node_inserted = false;
        while (!node_inserted) {
            if (key < curr->key) {
                if (curr->left == nullptr) {
                    curr->left = new Node(key, curr);
                    node_inserted = true;
                } else {
                    curr = curr->left;
                }
            } else if (key > curr->key) {
                if (curr->right == nullptr) {
                    curr->right = new Node(key, curr);
                    node_inserted = true;
                } else {
                    curr = curr->right;
                }
            } else {
                node_inserted = true;
            }
        }
    }

    Node *find(int key) const {
        Node *curr = root_;
        while (curr != nullptr) {
            if (key < curr->key) {
                curr = curr->left;
            } else if (key > curr->key) {
                curr = curr->right;
            } else {
                return curr;
            }
        }
        return nullptr;
    }

    int splay(Node *node_goes_up) {
        int rotations_number = 0;

        if (node_goes_up == root_) {
            return rotations_number;
        }

        Node *curr = node_goes_up;

        while (curr->parent) {
            if (!curr->parent->parent) {
                if (curr == curr->parent->left) {
                    // Малый правый поворот.
                    rightRotate(curr->parent);
                } else {
                    // Малый левый поворот.
                    leftRotate(curr->parent);
                }
                ++rotations_number;
            } else if (curr == curr->parent->left && curr->parent == curr->parent->parent->left) {
                // Два правых поворота.
                rightRotate(curr->parent->parent);
                rightRotate(curr->parent);
                rotations_number += 2;
            } else if (curr == curr->parent->right && curr->parent == curr->parent->parent->right) {
                // Два левых поворота.
                leftRotate(curr->parent->parent);
                leftRotate(curr->parent);
                rotations_number += 2;
            } else if (curr == curr->parent->right && curr->parent == curr->parent->parent->left) {
                // Большой правый поворот.
                leftRotate(curr->parent);
                rightRotate(curr->parent);
                ++rotations_number;
            } else {
                // Большой левый поворот.
                rightRotate(curr->parent);
                leftRotate(curr->parent);
                ++rotations_number;
            }
        }
        root_ = curr;
        return rotations_number;
    }

    int getHeight() const {
        return getHeightRecursive(root_) - 1;
    }
};
