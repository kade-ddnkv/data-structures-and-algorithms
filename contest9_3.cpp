#include <iostream>

class Node {
    int *keys_;
    int t_;
    Node **c_;
    int n_;
    bool leaf_;

public:
    Node(int degree, bool leaf) {
        t_ = degree;
        leaf_ = leaf;

        keys_ = new int[2 * t_ - 1];
        c_ = new Node *[2 * t_];

        n_ = 0;
    }

    void insertNonFull(int key) {
        int index = n_ - 1;

        if (leaf_) {
            while (index >= 0 && keys_[index] > key) {
                keys_[index + 1] = keys_[index];
                index--;
            }

            keys_[index + 1] = key;
            n_ = n_ + 1;
        } else {
            while (index >= 0 && keys_[index] > key) {
                index--;
            }

            if (c_[index + 1]->n_ == 2 * t_ - 1) {
                splitChild(index + 1, c_[index + 1]);

                if (keys_[index + 1] < key) {
                    index++;
                }
            }
            c_[index + 1]->insertNonFull(key);
        }
    }

    void splitChild(int index, Node *y) {
        Node *z = new Node(y->t_, y->leaf_);
        z->n_ = t_ - 1;

        for (int j = 0; j < t_ - 1; ++j) {
            z->keys_[j] = y->keys_[j + t_];
        }

        if (!y->leaf_) {
            for (int j = 0; j < t_; ++j) {
                z->c_[j] = y->c_[j + t_];
            }
        }

        y->n_ = t_ - 1;
        for (int j = n_; j >= index + 1; --j) {
            c_[j + 1] = c_[j];
        }

        c_[index + 1] = z;

        for (int j = n_ - 1; j >= index; --j) {
            keys_[j + 1] = keys_[j];
        }

        keys_[index] = y->keys_[t_ - 1];
        n_ = n_ + 1;
    }

    int64_t sum() {
        int64_t sum = 0;
        for (int64_t i = 0; i < n_; ++i) {
            sum += keys_[i];
        }
        return sum;
    }

    ~Node() {
        delete[] keys_;
        if (!leaf_) {
            for (int64_t i = 0; i < n_ + 1; ++i) {
                delete c_[i];
            }
        }
        delete[] c_;
    }

    friend class BTree;
};

class BTree {
    Node *root_;
    int t_;

    bool containsKey(int64_t key) {
        if (root_ == nullptr) {
            return false;
        }

        Node *node = root_;
        int index;
        while (!node->leaf_) {
            index = node->n_ - 1;
            while (index >= 0 && key <= node->keys_[index]) {
                if (key == node->keys_[index]) {
                    return true;
                }
                --index;
            }
            ++index;
            node = node->c_[index];
        }
        for (int i = 0; i < node->n_; ++i) {
            if (node->keys_[i] == key) {
                return true;
            }
        }
        return false;
    }

    int64_t getSizeRecursive(Node *curr) const {
        if (curr->leaf_) {
            return 1;
        }
        int64_t nodes_count = 1;
        for (int64_t i = 0; i < curr->n_ + 1; ++i) {
            nodes_count += getSizeRecursive(curr->c_[i]);
        }
        return nodes_count;
    }

    int64_t getSumRecursive(Node *curr) const {
        if (curr->leaf_) {
            return curr->sum();
        }
        int64_t curr_sum = curr->sum();
        for (int64_t i = 0; i < curr->n_ + 1; ++i) {
            curr_sum += getSumRecursive(curr->c_[i]);
        }
        return curr_sum;
    }

public:
    explicit BTree(int degree) {
        root_ = nullptr;
        t_ = degree;
    }

    ~BTree() {
        delete root_;
    }

    void insert(int key) {
        if (containsKey(key)) {
            return;
        }
        if (root_ == nullptr) {
            root_ = new Node(t_, true);
            root_->keys_[0] = key;
            root_->n_ = 1;
        } else {
            if (root_->n_ == 2 * t_ - 1) {
                Node *s = new Node(t_, false);

                s->c_[0] = root_;

                s->splitChild(0, root_);

                int index = 0;
                if (s->keys_[0] < key) {
                    index++;
                }
                s->c_[index]->insertNonFull(key);

                root_ = s;
            } else {
                root_->insertNonFull(key);
            }
        }
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
};

int main() {
    BTree t(2);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    t.insert(11);
    t.insert(15);
    t.insert(16);
    t.insert(17);
    t.insert(18);
    t.insert(20);
    t.insert(23);

    std::cout << t.size() << "\n";
    std::cout << t.sum() << "\n";
}