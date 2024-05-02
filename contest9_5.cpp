#include <iostream>

class BPlusTree {
    class Node {
        friend BPlusTree;
        int64_t *keys_;
        Node **child_ptrs_;
        // Node *parent_;

        int64_t size_;
        int64_t degree_;

        bool is_leaf_;
        Node *next_leaf_ = nullptr;

    public:
        explicit Node(int64_t degree) {
            degree_ = degree;
            keys_ = new int64_t[maxSize()];
            child_ptrs_ = new Node *[maxSize() + 1];
        }

        ~Node() {
            delete[] keys_;
            if (!is_leaf_) {
                for (int64_t i = 0; i < childrenCount(); ++i) {
                    delete child_ptrs_[i];
                }
            }
            delete[] child_ptrs_;
        }

        // Возвращает индекс вставки.
        int insertKey(int64_t key) {
            int index_to_place = size_ - 1;
            while (index_to_place >= 0 && key < keys_[index_to_place]) {
                --index_to_place;
            }
            ++index_to_place;
            for (int i = size_ - 1; i >= index_to_place; --i) {
                keys_[i + 1] = keys_[i];
            }
            keys_[index_to_place] = key;
            ++size_;
            return index_to_place;
        }

        void insertKeyWithChild(int64_t key, Node *child) {
            int index_to_place = insertKey(key);
            if (child != nullptr) {
                // size уже был увеличен на 1.
                ++index_to_place;
                for (int i = size_ - 1; i >= index_to_place; --i) {
                    child_ptrs_[i + 1] = child_ptrs_[i];
                }
                child_ptrs_[index_to_place] = child;
            }
        }

        int64_t maxSize() {
            return 2 * degree_ - 1;
        }

        int64_t childrenCount() {
            return size_ + 1;
        }

        bool isFull() {
            return size_ == maxSize();
        }

        void print() {
            for (int i = 0; i < size_; ++i) {
                std::cout << keys_[i] << " ";
            }
            std::cout << "\n";
        }
    };

    int degree_;
    int max_size_;
    int max_children_number_;
    Node *root_ = nullptr;

    // Insert Operation
    void insertOperation(int key) {
        if (root_ == nullptr) {
            root_ = new Node(degree_);
            root_->keys_[0] = key;
            root_->is_leaf_ = true;
            root_->size_ = 1;
        } else {
            Node *cursor = root_;
            Node *parent;
            while (!cursor->is_leaf_) {
                parent = cursor;
                for (int i = 0; i < cursor->size_; ++i) {
                    if (key < cursor->keys_[i]) {
                        cursor = cursor->child_ptrs_[i];
                        break;
                    }
                    if (i == cursor->size_ - 1) {
                        cursor = cursor->child_ptrs_[i + 1];
                        break;
                    }
                }
            }
            if (cursor->size_ < max_size_) {
                int i_index = 0;
                while (key > cursor->keys_[i_index] && i_index < cursor->size_) {
                    i_index++;
                }
                for (int j = cursor->size_; j > i_index; --j) {
                    cursor->keys_[j] = cursor->keys_[j - 1];
                }
                cursor->keys_[i_index] = key;
                cursor->size_++;
                cursor->child_ptrs_[cursor->size_] = cursor->child_ptrs_[cursor->size_ - 1];
                cursor->child_ptrs_[cursor->size_ - 1] = nullptr;
            } else {
                Node *new_leaf = new Node(degree_);
                int virtual_node[max_size_ + 1];
                for (int i = 0; i < max_size_; ++i) {
                    virtual_node[i] = cursor->keys_[i];
                }
                int i_index = 0, j_index;
                while (key > virtual_node[i_index] && i_index < max_size_) {
                    i_index++;
                }
                for (int j = max_size_; j > i_index; j--) {
                    virtual_node[j] = virtual_node[j - 1];
                }
                virtual_node[i_index] = key;
                new_leaf->is_leaf_ = true;
                cursor->size_ = (max_size_ + 1) / 2;
                new_leaf->size_ = max_size_ + 1 - (max_size_ + 1) / 2;
                cursor->child_ptrs_[cursor->size_] = new_leaf;
                new_leaf->child_ptrs_[new_leaf->size_] = cursor->child_ptrs_[max_size_];
                cursor->child_ptrs_[max_size_] = nullptr;
                for (i_index = 0; i_index < cursor->size_; ++i_index) {
                    cursor->keys_[i_index] = virtual_node[i_index];
                }
                for (i_index = 0, j_index = cursor->size_; i_index < new_leaf->size_;
                     ++i_index, ++j_index) {
                    new_leaf->keys_[i_index] = virtual_node[j_index];
                }

                Node *prev_right_node = cursor->next_leaf_;
                cursor->next_leaf_ = new_leaf;
                new_leaf->next_leaf_ = prev_right_node;

                if (cursor == root_) {
                    Node *new_root = new Node(degree_);
                    new_root->keys_[0] = new_leaf->keys_[0];
                    new_root->child_ptrs_[0] = cursor;
                    new_root->child_ptrs_[1] = new_leaf;
                    new_root->is_leaf_ = false;
                    new_root->size_ = 1;
                    root_ = new_root;
                } else {
                    insertInternal(new_leaf->keys_[0], parent, new_leaf);
                }
            }
        }
    }

    // Insert Operation
    void insertInternal(int x, Node *cursor, Node *child) {
        if (cursor->size_ < max_size_) {
            int i = 0;
            while (x > cursor->keys_[i] && i < cursor->size_) {
                i++;
            }
            for (int j = cursor->size_; j > i; j--) {
                cursor->keys_[j] = cursor->keys_[j - 1];
            }
            for (int j = cursor->size_ + 1; j > i + 1; j--) {
                cursor->child_ptrs_[j] = cursor->child_ptrs_[j - 1];
            }
            cursor->keys_[i] = x;
            cursor->size_++;
            cursor->child_ptrs_[i + 1] = child;
        } else {
            Node *new_internal = new Node(degree_);
            int virtual_key[max_size_ + 1];
            Node *virtual_ptr[max_size_ + 2];
            for (int i = 0; i < max_size_; ++i) {
                virtual_key[i] = cursor->keys_[i];
            }
            for (int i = 0; i < max_size_ + 1; ++i) {
                virtual_ptr[i] = cursor->child_ptrs_[i];
            }
            int i_index = 0, j_index;
            while (x > virtual_key[i_index] && i_index < max_size_) {
                i_index++;
            }
            for (int j = max_size_; j > i_index; j--) {
                virtual_key[j] = virtual_key[j - 1];
            }
            virtual_key[i_index] = x;
            for (int j = max_size_ + 1; j > i_index + 1; j--) {
                virtual_ptr[j] = virtual_ptr[j - 1];
            }
            virtual_ptr[i_index + 1] = child;
            new_internal->is_leaf_ = false;
            cursor->size_ = (max_size_ + 1) / 2;
            new_internal->size_ = max_size_ - (max_size_ + 1) / 2;
            for (i_index = 0, j_index = cursor->size_ + 1; i_index < new_internal->size_;
                 ++i_index, ++j_index) {
                new_internal->keys_[i_index] = virtual_key[j_index];
            }
            for (i_index = 0, j_index = cursor->size_ + 1; i_index < new_internal->size_ + 1;
                 ++i_index, ++j_index) {
                new_internal->child_ptrs_[i_index] = virtual_ptr[j_index];
            }
            if (cursor == root_) {
                Node *new_root = new Node(degree_);
                new_root->keys_[0] = cursor->keys_[cursor->size_];
                new_root->child_ptrs_[0] = cursor;
                new_root->child_ptrs_[1] = new_internal;
                new_root->is_leaf_ = false;
                new_root->size_ = 1;
                root_ = new_root;
            } else {
                insertInternal(cursor->keys_[cursor->size_], findParent(root_, cursor),
                               new_internal);
            }
        }
    }

    // Find the parent
    Node *findParent(Node *cursor, Node *child) {
        Node *parent;
        if (cursor->is_leaf_ || (cursor->child_ptrs_[0])->is_leaf_) {
            return nullptr;
        }
        for (int i = 0; i < cursor->size_ + 1; ++i) {
            if (cursor->child_ptrs_[i] == child) {
                parent = cursor;
                return parent;
            } else {
                parent = findParent(cursor->child_ptrs_[i], child);
                if (parent != nullptr) {
                    return parent;
                }
            }
        }
        return parent;
    }

    Node *findRecursive(Node *curr, int64_t key_to_find) {
        // Если текущий узел - лист, проверяю на наличие нужного ключа.
        if (curr->is_leaf_) {
            for (int64_t i = 0; i < curr->size_; ++i) {
                if (curr->keys_[i] == key_to_find) {
                    return curr;
                }
            }
            return nullptr;
        }

        // Проход по ключам, проверка и запуск от нужного потомка.
        for (int64_t i = 0; i < curr->size_; ++i) {
            if (key_to_find < curr->keys_[i]) {
                return findRecursive(curr->child_ptrs_[i], key_to_find);
            } else if (key_to_find == curr->keys_[i]) {
                return curr;
            }
        }

        // Если не было запущено ранее, поиск в самом правом потомке.
        return findRecursive(curr->child_ptrs_[curr->childrenCount() - 1], key_to_find);
    }

    bool containsKey(int64_t key) {
        if (root_ == nullptr) {
            return false;
        }
        return findRecursive(root_, key) != nullptr;
    }

    Node *searchForKeyPlace(int64_t key_to_insert) const {
        // Корень непустой.
        Node *cursor = root_;
        // Цикл находит подходящий для вставки лист.
        while (!cursor->is_leaf_) {
            int index_of_child = cursor->size_ - 1;
            while (index_of_child >= 0 && key_to_insert < cursor->keys_[index_of_child]) {
                --index_of_child;
            }
            ++index_of_child;
            cursor = cursor->child_ptrs_[index_of_child];
        }
        return cursor;
    }

public:
    explicit BPlusTree(int degree) {
        degree_ = degree;
        max_size_ = 2 * degree_ - 1;
        max_children_number_ = max_size_ + 1;
    }

    ~BPlusTree() {
        delete root_;
    }

    void insert(int64_t key) {
        if (containsKey(key)) {
            return;
        }
        /*
        if (root_ == nullptr) {
            root_ = new Node(degree_);
            root_->keys_[0] = key;
            root_->size_ = 1;
            root_->is_leaf_ = true;
            root_->parent_ = nullptr;
            return;
        }
        Node *cursor = searchForKeyPlace(key);*/
        insertOperation(key);
    }

    void printLeaves() const {
        if (root_ == nullptr) {
            return;
        }
        const int k_min_value = -2147483648;
        Node *cursor = searchForKeyPlace(k_min_value);
        while (cursor != nullptr) {
            cursor->print();
            cursor = cursor->next_leaf_;
        }
    }
};
