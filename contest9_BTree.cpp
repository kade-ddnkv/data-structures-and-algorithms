#include <iostream>

// Алгоритм вставки взят из книги Кормена.

class BTree {
    class Node {
        friend BTree;
        int64_t *keys_;
        Node **child_ptrs_;

        int64_t size_;
        int64_t degree_;
        bool is_leaf_;

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

        int64_t maxSize() {
            return 2 * degree_ - 1;
        }

        int64_t childrenCount() {
            return size_ + 1;
        }

        bool isFull() {
            return size_ == maxSize();
        }

        int64_t sum() {
            int64_t sum = 0;
            for (int64_t i = 0; i < size_; ++i) {
                sum += keys_[i];
            }
            return sum;
        }
    };

    int64_t degree_;
    Node *root_ = nullptr;

    void splitChild(Node *parent, int64_t index_of_child) {
        // parent - незаполненный узел.
        // index_of_child - индекс его заполненного потомка, которого нужно разделить.
        Node *new_child = new Node(degree_);
        Node *child = parent->child_ptrs_[index_of_child];
        new_child->is_leaf_ = child->is_leaf_;
        new_child->size_ = degree_ - 1;
        // Копирование половины ключей.
        for (int j = 0; j < degree_ - 1; ++j) {
            new_child->keys_[j] = child->keys_[j + degree_];
        }
        // Копирование половины ссылок на потомков.
        if (!child->is_leaf_) {
            for (int j = 0; j < degree_; ++j) {
                new_child->child_ptrs_[j] = child->child_ptrs_[j + degree_];
            }
        }
        child->size_ = degree_ - 1;
        // Перенос ключей и вставка нового ключа в родителя.
        for (int j = parent->size_; j >= index_of_child + 1; --j) {
            parent->child_ptrs_[j + 1] = parent->child_ptrs_[j];
        }
        parent->child_ptrs_[index_of_child + 1] = new_child;
        // Перенос потомков и вставка нового потомка в родителя.
        for (int j = parent->size_ - 1; j >= index_of_child; --j) {
            parent->keys_[j + 1] = parent->keys_[j];
        }
        parent->keys_[index_of_child] = child->keys_[degree_ - 1];
        ++parent->size_;
    }

    void insertToNonFull(Node *node, int64_t key_to_insert) {
        int64_t index_of_child = node->size_ - 1;
        // Если текущий узел - лист, он обязательно неполный.
        if (node->is_leaf_) {
            // Перенос ключей и вставка нового.
            while (index_of_child >= 0 && key_to_insert < node->keys_[index_of_child]) {
                node->keys_[index_of_child + 1] = node->keys_[index_of_child];
                --index_of_child;
            }
            node->keys_[index_of_child + 1] = key_to_insert;
            ++node->size_;
        } else {
            // Находим нужного потомка.
            while (index_of_child >= 0 && key_to_insert < node->keys_[index_of_child]) {
                --index_of_child;
            }
            ++index_of_child;
            // Если нужный потомок полный, разделить его.
            if (node->child_ptrs_[index_of_child]->isFull()) {
                splitChild(node, index_of_child);
                if (key_to_insert > node->keys_[index_of_child]) {
                    ++index_of_child;
                }
            }
            insertToNonFull(node->child_ptrs_[index_of_child], key_to_insert);
        }
    }

    void insertToRoot(int64_t key_to_insert) {
        // Возможно разделение корня.
        if (root_->isFull()) {
            Node *new_root = new Node(degree_);
            new_root->is_leaf_ = false;
            new_root->size_ = 0;
            new_root->child_ptrs_[0] = root_;
            root_ = new_root;
            splitChild(new_root, 0);
            insertToNonFull(new_root, key_to_insert);
        } else {
            insertToNonFull(root_, key_to_insert);
        }
    }

    int64_t getSizeRecursive(Node *curr) const {
        if (curr->is_leaf_) {
            return 1;
        }
        int64_t nodes_count = 1;
        for (int64_t i = 0; i < curr->childrenCount(); ++i) {
            nodes_count += getSizeRecursive(curr->child_ptrs_[i]);
        }
        return nodes_count;
    }

    int64_t getSumRecursive(Node *curr) const {
        if (curr->is_leaf_) {
            return curr->sum();
        }
        int64_t curr_sum = 0;
        for (int64_t i = 0; i < curr->childrenCount(); ++i) {
            curr_sum += getSumRecursive(curr->child_ptrs_[i]);
        }
        return curr_sum;
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
    explicit BTree(int64_t degree) {
        degree_ = degree;
    }

    ~BTree() {
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
            return;
        }
        insertToRoot(key);
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
