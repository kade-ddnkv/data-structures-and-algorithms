#include <iostream>

class BTree {
    class Node {
        friend BTree;
        int64_t *data_;
        Node **child_ptrs_;
        Node *parent_;

        int64_t size_;
        int64_t rank_;
        bool is_leaf_;

    public:
        Node(int64_t key, Node **child_ptrs, int64_t rank, bool is_leaf) {
            rank_ = rank;
            data_ = new int64_t[maxSize()];
            data_[0] = key;
            child_ptrs_ = child_ptrs;
            parent_ = nullptr;
            size_ = 1;
            is_leaf_ = is_leaf;
        }

        Node(int64_t *data, Node **child_ptrs, Node *parent, int64_t size, int64_t rank,
             bool is_leaf) {
            rank_ = rank;
            data_ = data;
            child_ptrs_ = child_ptrs;
            parent_ = parent;
            size_ = size;
            is_leaf_ = is_leaf;
        }

        ~Node() {
            delete[] data_;
            if (!is_leaf_) {
                for (int i = 0; i < childrenCount(); ++i) {
                    delete child_ptrs_[i];
                }
            }
            delete[] child_ptrs_;
        }

        int64_t maxSize() {
            return 2 * rank_ - 1;
        }

        int64_t childrenCount() {
            return size_ + 1;
        }

        void insertValueWithChildToArray(int64_t value, Node *corresponding_child) {
            // Предполагается, что в массиве есть свободное место.
            bool is_placed = false;
            for (int64_t i = 0; i < size_; ++i) {
                if (value < data_[i]) {
                    for (int64_t j = size_; j > i; --j) {
                        data_[j] = data_[j - 1];
                    }
                    data_[i] = value;
                    is_placed = true;
                    break;
                }
            }
            // Если до этого момента не была произведена вставка,
            // значит, элемент вставляется последним.
            if (!is_placed) {
                data_[size_] = value;
            }
            ++size_;
            insertPtrToChildrenArray(corresponding_child);
        }

        void insertPtrToChildrenArray(Node *child) {
            // Эта функция вызывается из insertValueWithChildToArray.
            // Значение size уже увеличено на 1 (сейчас одинаковое кол-во данных и дочерних узлов).
            // Предполагается, что есть место для еще одного потомка.
            if (child == nullptr) {
                return;
            }
            child->parent_ = this;
            int64_t first_element = child->data_[0];
            for (int64_t i = 0; i < size_; ++i) {
                if (first_element < data_[i]) {
                    for (int64_t j = size_; j > i; --j) {
                        child_ptrs_[j] = child_ptrs_[j - 1];
                    }
                    child_ptrs_[i] = child;
                    return;
                }
            }
            child_ptrs_[size_] = child;
        }

        int64_t sum() {
            int64_t sum = 0;
            for (int64_t i = 0; i < size_; ++i) {
                sum += data_[i];
            }
            return sum;
        }

        bool contains(int64_t key) {
            for (int i = 0; i < size_; ++i) {
                if (data_[i] == key) {
                    return true;
                }
            }
            return false;
        }
    };

    int64_t rank_;
    int64_t max_size_;
    int64_t max_children_number_;
    Node *root_ = nullptr;

    void splitIntoTwoNodes(Node *node) {
        // В заполненном узле всегда нечетное количество значений.
        // Значение посередине.
        int64_t index_of_middle = max_size_ / 2;
        int64_t key_goes_up = node->data_[index_of_middle];

        node->size_ = index_of_middle;
        int64_t *second_node_data = new int64_t[max_size_];
        for (int64_t i = 0; i < index_of_middle; ++i) {
            second_node_data[i] = node->data_[i + index_of_middle + 1];
        }

        Node **second_node_children = nullptr;
        Node *second_node = new Node(second_node_data, second_node_children, node->parent_,
                                     index_of_middle, rank_, node->is_leaf_);
        if (!node->is_leaf_) {
            second_node_children = new Node *[max_children_number_];
            for (int64_t i = 0; i < max_size_ / 2 + 1; ++i) {
                second_node_children[i] = node->child_ptrs_[i + index_of_middle + 1];
                second_node_children[i]->parent_ = second_node;
            }
            second_node->child_ptrs_ = second_node_children;
        }

        // Добавляю серединный нод в родительский узел.
        if (node->parent_ == nullptr) {
            root_ = new Node(key_goes_up, new Node *[max_children_number_] { node, second_node },
                             rank_, false);
            node->parent_ = root_;
            second_node->parent_ = root_;
        } else {
            insertValueToNode(node->parent_, key_goes_up, second_node);
            // node->parent_->insertPtrToChildrenArray(second_node);
        }
    }

    void insertValueToChild(Node *node, int64_t value, Node *corresponding_child) {
        // Предполагается, что у нужных детей будет пустое место.
        for (int64_t i = 0; i < node->size_; ++i) {
            if (value < node->data_[i]) {
                node->child_ptrs_[i]->insertValueWithChildToArray(value, corresponding_child);
                return;
            }
        }
        // Если не было вставлено ранее, то вставка производится в самого правого потомка.
        Node *last_child = node->child_ptrs_[node->childrenCount() - 1];
        last_child->insertValueWithChildToArray(value, corresponding_child);
    }

    void insertValueToNode(Node *node, int64_t value, Node *corresponding_child) {
        if (node->size_ < max_size_) {
            // Вставка элемента в массив.
            node->insertValueWithChildToArray(value, corresponding_child);
        } else {
            splitIntoTwoNodes(node);
            insertValueToChild(node->parent_, value, corresponding_child);
        }
    }

    void insertRecursive(Node *curr, int64_t key_to_insert) {
        if (curr->is_leaf_) {
            if (!curr->contains(key_to_insert)) {
                insertValueToNode(curr, key_to_insert, nullptr);
            }
            return;
        }

        for (int64_t i = 0; i < curr->size_; ++i) {
            if (key_to_insert < curr->data_[i]) {
                insertRecursive(curr->child_ptrs_[i], key_to_insert);
                return;
            } else if (key_to_insert == curr->data_[i]) {
                return;
            }
        }
        // Если не было вставлено ранее, то вставка производится в самого правого потомка.
        insertRecursive(curr->child_ptrs_[curr->childrenCount() - 1], key_to_insert);
    }

    int64_t getSizeRecursive(Node *curr) const {
        if (curr->is_leaf_) {
            return 1;
        }
        int64_t nodes_count = 1;
        for (int i = 0; i < curr->childrenCount(); ++i) {
            nodes_count += getSizeRecursive(curr->child_ptrs_[i]);
        }
        return nodes_count;
    }

    int64_t getSumRecursive(Node *curr) const {
        if (curr->is_leaf_) {
            return curr->sum();
        }
        int64_t curr_sum = curr->sum();
        for (int64_t i = 0; i < curr->childrenCount(); ++i) {
            curr_sum += getSumRecursive(curr->child_ptrs_[i]);
        }
        return curr_sum;
    }

    void printNode(Node *node) {
        std::cout << "(";
        for (int i = 0; i < node->size_ - 1; ++i) {
            std::cout << node->data_[i] << ", ";
        }
        std::cout << node->data_[node->size_ - 1];
        std::cout << ")";
    }

    void printTreeRecursive(Node *curr, int spaces_number) {
        if (!curr->is_leaf_) {
            for (int i = 0; i < curr->childrenCount() / 2; ++i) {
                printTreeRecursive(curr->child_ptrs_[i], spaces_number + 5);
            }
            for (int i = 0; i < spaces_number; ++i) {
                std::cout << " ";
            }
            printNode(curr);
            std::cout << "\n";
            for (int i = curr->childrenCount() / 2; i < curr->childrenCount(); ++i) {
                printTreeRecursive(curr->child_ptrs_[i], spaces_number + 5);
            }
        } else {
            for (int i = 0; i < spaces_number; ++i) {
                std::cout << " ";
            }
            printNode(curr);
            std::cout << "\n";
        }
    }

public:

    void printTree() {
        printTreeRecursive(root_, 0);
        std::cout << "\n";
    }
    explicit BTree(int64_t rank) {
        rank_ = rank;
        max_size_ = 2 * rank_ - 1;
        max_children_number_ = max_size_ + 1;
    }

    ~BTree() {
        delete root_;
    }

    void insert(int64_t key) {
        if (root_ == nullptr) {
            root_ = new Node(key, nullptr, rank_, true);
            return;
        }
        insertRecursive(root_, key);
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
        // Hello.
    }
};

int main() {
    BTree b_tree(2);
    for (int i = 20; i >= 1; --i) {
        b_tree.insert(i);
    }

    b_tree.printTree();
    std::cout << b_tree.size() << "\n";
    std::cout << b_tree.sum() << "\n";

    return 0;
}