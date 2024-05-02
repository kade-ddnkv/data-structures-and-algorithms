#include <iostream>

class BPlusTree {
    class Node {
        friend BPlusTree;
        int64_t *keys_;
        Node **child_ptrs_;
        Node *parent_;

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
    Node *root_ = nullptr;

    int insertKeyToArray(int64_t key, int64_t *array, int64_t array_size) {
        int index_to_place = array_size - 2;
        while (index_to_place >= 0 && key < array[index_to_place]) {
            --index_to_place;
        }
        ++index_to_place;
        for (int i = array_size - 2; i >= index_to_place; --i) {
            array[i + 1] = array[i];
        }
        array[index_to_place] = key;
        return index_to_place;
    }

    Node *splitNode(Node *left, int64_t key_to_insert, Node *corresponding_child) {
        // В узле нечетное количество ключей (2t - 1).
        Node *right = new Node(degree_);
        right->parent_ = left->parent_;
        right->is_leaf_ = left->is_leaf_;

        int key_goes_up;
        if (left->is_leaf_) {
            Node *prev_next_leaf = left->next_leaf_;
            left->next_leaf_ = right;
            right->next_leaf_ = prev_next_leaf;
            // В новый лист переместятся правые t ключей.
            // В прошлом листе останется t - 1 ключей.
            right->size_ = degree_;
            left->size_ = degree_ - 1;
            for (int i = 0; i < degree_; ++i) {
                right->keys_[i] = left->keys_[i + degree_ - 1];
            }
            key_goes_up = right->keys_[0];
            if (key_to_insert < right->keys_[0]) {
                left->insertKeyWithChild(key_to_insert, corresponding_child);
            } else {
                right->insertKeyWithChild(key_to_insert, corresponding_child);
            }

        } else {
            // В новый узел переместятся правые t - 1 ключей.
            // В прошлом листе останется t - 1 ключей.
            // Серединный ключ уходит наверх
            int64_t *virtual_keys = new int64_t[max_size_ + 1];
            Node **virtual_children = new Node *[max_size_ + 2];
            for (int i = 0; i < left->size_; ++i) {
                virtual_keys[i] = left->keys_[i];
            }
            for (int i = 0; i < left->childrenCount(); ++i) {
                virtual_children[i] = left->child_ptrs_[i];
            }

            int index_to_place = insertKeyToArray(key_to_insert, virtual_keys, max_size_ + 1);
            ++index_to_place;
            for (int i = max_size_; i >= index_to_place; --i) {
                virtual_children[i + 1] = virtual_children[i];
            }
            virtual_children[index_to_place] = corresponding_child;

            key_goes_up = virtual_keys[degree_ - 1];

            left->size_ = degree_ - 1;
            for (int i = 0; i < degree_ - 1; ++i) {
                left->keys_[i] = virtual_keys[i];
            }
            right->size_ = degree_;
            for (int i = 0; i < degree_; ++i) {
                right->keys_[i] = virtual_keys[i + degree_];
            }
            // Слева t потомков.
            for (int i = 0; i < degree_; ++i) {
                left->child_ptrs_[i] = virtual_children[i];
            }
            // Справа t + 1 потомков.
            for (int i = 0; i < degree_ + 1; ++i) {
                right->child_ptrs_[i] = virtual_children[i + degree_];
            }

            delete[] virtual_keys;
            delete[] virtual_children;
        }

        // После разделения узла на два нужно передать один ключ в нод наверх.
        if (left->parent_ == nullptr) {
            Node *new_root = new Node(degree_);
            new_root->keys_[0] = key_goes_up;
            new_root->child_ptrs_[0] = left;
            new_root->child_ptrs_[1] = right;
            new_root->is_leaf_ = false;
            new_root->size_ = 1;
            root_ = new_root;
            root_->parent_ = nullptr;
            left->parent_ = root_;
            right->parent_ = root_;
        } else {
            insertInternal(key_goes_up, right->parent_, right);
        }

        return right;
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

    void insertInternal(int64_t key_to_insert, Node *cursor, Node *corresponding_child) {
        // Если узел незаполненный, вставить ключ.
        if (!cursor->isFull()) {
            cursor->insertKeyWithChild(key_to_insert, corresponding_child);
            // Иначе разделить узел на два и отправить вверх ключ для вставки.
        } else {
            splitNode(cursor, key_to_insert, corresponding_child);
        }
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

public:
    explicit BPlusTree(int degree) {
        degree_ = degree;
        max_size_ = 2 * degree_ - 1;
    }

    ~BPlusTree() {
        delete root_;
    }

    void insert(int64_t key) {
        if (containsKey(key)) {
            return;
        }
        if (root_ == nullptr) {
            root_ = new Node(degree_);
            root_->keys_[0] = key;
            root_->size_ = 1;
            root_->is_leaf_ = true;
            root_->parent_ = nullptr;
            return;
        }
        Node *cursor = searchForKeyPlace(key);
        insertInternal(key, cursor, nullptr);
    }

    void printLeaves() const {
        if (root_ == nullptr) {
            return;
        }
        // Минимальное значение int32_t.
        const int k_min_value = -2147483648;
        Node *cursor = searchForKeyPlace(k_min_value);
        while (cursor != nullptr) {
            cursor->print();
            cursor = cursor->next_leaf_;
        }
    }
};
