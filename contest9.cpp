#include <iostream>

class BTree {
    class Node {
        friend BTree;
        int64_t *data_;
        Node **child_ptrs_;

        int64_t size_;
        int64_t degree_;
        bool is_leaf_;

    public:
        Node(int64_t key, Node **child_ptrs, int64_t degree, bool is_leaf) {
            degree_ = degree;
            data_ = new int64_t[maxSize()];
            data_[0] = key;
            child_ptrs_ = child_ptrs;
            size_ = 1;
            is_leaf_ = is_leaf;
        }

        Node(int64_t *data, Node **child_ptrs, int64_t size, int64_t degree, bool is_leaf) {
            degree_ = degree;
            data_ = data;
            child_ptrs_ = child_ptrs;
            size_ = size;
            is_leaf_ = is_leaf;
        }

        ~Node() {
            delete[] data_;
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

        void insertValueToArray(int64_t value) {
            // Предполагается, что в массиве есть свободное место.
            for (int64_t i = 0; i < size_; ++i) {
                if (value < data_[i]) {
                    for (int64_t j = size_; j > i; --j) {
                        data_[j] = data_[j - 1];
                    }
                    data_[i] = value;
                    ++size_;
                    return;
                }
            }
            // Если до этого момента не была произведена вставка,
            // значит, элемент вставляется последним.
            data_[size_] = value;
            ++size_;
        }

        void addPtrToChildrenArray(Node *child_remains, Node *child_to_add) {
            // Предполагается, что в child_ptrs_ есть child_remains.
            for (int64_t i = 0; i < size_; ++i) {
                if (child_ptrs_[i] == child_remains) {
                    for (int64_t j = size_; j > i; --j) {
                        child_ptrs_[j] = child_ptrs_[j - 1];
                    }
                    child_ptrs_[i + 1] = child_to_add;
                    return;
                }
            }
            child_ptrs_[size_] = child_to_add;
        }

        bool isFull() {
            return size_ == maxSize();
        }

        int64_t sum() {
            int64_t sum = 0;
            for (int64_t i = 0; i < size_; ++i) {
                sum += data_[i];
            }
            return sum;
        }

        bool contains(int64_t key) {
            for (int64_t i = 0; i < size_; ++i) {
                if (data_[i] == key) {
                    return true;
                }
            }
            return false;
        }
    };

    struct ResultAfterSplit {
        int64_t key_goes_up;
        Node *left_child;
        Node *right_child;
    };

    int64_t degree_;
    int64_t max_size_;
    int64_t max_children_number_;
    int64_t insert_invoke_count_ = 0;
    int64_t real_sum_ = 0;
    int64_t equal_keys_insertion_ = 0;
    Node *root_ = nullptr;

    ResultAfterSplit splitIntoTwoNodes(Node *node) {
        int64_t index_of_middle = max_size_ / 2;

        ResultAfterSplit split_result;
        split_result.key_goes_up = node->data_[index_of_middle];

        node->size_ = degree_ - 1;
        int64_t second_node_size = degree_ - 1;
        int64_t *second_node_data = new int64_t[max_size_];
        for (int64_t i = 0; i < degree_ - 1; ++i) {
            second_node_data[i] = node->data_[i + index_of_middle + 1];
        }

        Node **second_node_children = nullptr;
        Node *second_node = new Node(second_node_data, second_node_children, second_node_size,
                                     degree_, node->is_leaf_);
        if (!node->is_leaf_) {
            second_node_children = new Node *[max_children_number_];
            for (int64_t i = 0; i < degree_; ++i) {
                second_node_children[i] = node->child_ptrs_[i + index_of_middle + 1];
            }
            second_node->child_ptrs_ = second_node_children;
        }

        split_result.left_child = node;
        split_result.right_child = second_node;
        return split_result;
    }

    /*
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
                                     index_of_middle, degree_, node->is_leaf_);
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
            root_ = new Node(
                key_goes_up, new Node *[max_children_number_] { node, second_node }, degree_,
                false);
            node->parent_ = root_;
            second_node->parent_ = root_;
        } else {
            node->parent_->insertValueToArray(key_goes_up);
            node->parent_->insertPtrToChildrenArray(second_node);
        }
    }
     */

    void insertRecursive(Node *curr, int64_t key_to_insert) {
        if (curr->is_leaf_) {
            curr->insertValueToArray(key_to_insert);
        } else {
            // Пункт 1.
            Node *child_insert_to;
            bool child_found = false;
            for (int64_t i = 0; !child_found && i < curr->size_; ++i) {
                if (key_to_insert < curr->data_[i]) {
                    child_insert_to = curr->child_ptrs_[i];
                    child_found = true;
                }
            }
            if (!child_found) {
                child_insert_to = curr->child_ptrs_[curr->childrenCount() - 1];
            }

            // Пункт 2.
            insertRecursive(child_insert_to, key_to_insert);

            // Пункт 3.
            if (child_insert_to->isFull()) {
                ResultAfterSplit res = splitIntoTwoNodes(child_insert_to);
                curr->insertValueToArray(res.key_goes_up);
                curr->addPtrToChildrenArray(res.left_child, res.right_child);
            }
        }
    }

    /*
    void insertRecursive(Node *curr, int64_t key_to_insert) {
        if (curr->isFull()) {
            splitIntoTwoNodes(curr);
            insertRecursive(curr->parent_, key_to_insert);
            return;
        }

        if (curr->is_leaf_) {
            curr->insertValueToArray(key_to_insert);
            return;
        }

        for (int64_t i = 0; i < curr->size_; ++i) {
            if (key_to_insert < curr->data_[i]) {
                insertRecursive(curr->child_ptrs_[i], key_to_insert);
                return;
            }
        }
        // Если не было вставлено ранее, то вставка производится в самого правого потомка.
        insertRecursive(curr->child_ptrs_[curr->childrenCount() - 1], key_to_insert);
    }
     */

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
        int64_t curr_sum = curr->sum();
        for (int64_t i = 0; i < curr->childrenCount(); ++i) {
            curr_sum += getSumRecursive(curr->child_ptrs_[i]);
        }
        return curr_sum;
    }

    Node *findRecursive(Node *curr, int64_t key_to_find) {
        if (curr->is_leaf_) {
            for (int64_t i = 0; i < curr->size_; ++i) {
                if (curr->data_[i] == key_to_find) {
                    return curr;
                }
            }
            return nullptr;
        }

        for (int64_t i = 0; i < curr->size_; ++i) {
            if (key_to_find < curr->data_[i]) {
                return findRecursive(curr->child_ptrs_[i], key_to_find);
            } else if (key_to_find == curr->data_[i]) {
                return curr;
            }
        }
        // Если не было вставлено ранее, то вставка производится в самого правого потомка.
        return findRecursive(curr->child_ptrs_[curr->childrenCount() - 1], key_to_find);
    }

    bool containsKey(int64_t key) {
        if (root_ == nullptr) {
            return false;
        }
        return findRecursive(root_, key) != nullptr;
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

    /*
    void printTreeRecursiveObsolete(Node *curr) {
        std::cout << "[ ";
        for (int64_t i = 0; i < curr->size_; ++i) {
            std::cout << curr->data_[i] << " ";
        }
        std::cout << " ]";
        if (!curr->is_leaf_) {
            std::cout << "( ";
            for (int64_t i = 0; i < curr->childrenCount(); ++i) {
                printTreeRecursive(curr->child_ptrs_[i]);
            }
            std::cout << " )";
        }
    }
     */

public:

    void printTree() {
        printTreeRecursive(root_, 0);
        std::cout << "\n";
    }
    explicit BTree(int64_t rank) {
        degree_ = rank;
        max_size_ = 2 * degree_ - 1;
        max_children_number_ = max_size_ + 1;
    }

    ~BTree() {
        delete root_;
    }

    void insert(int64_t key) {
        ++insert_invoke_count_;
        real_sum_ += key;
        if (containsKey(key)) {
            ++equal_keys_insertion_;
            return;
        }
        if (root_ == nullptr) {
            root_ = new Node(key, nullptr, degree_, true);
            return;
        }
        insertRecursive(root_, key);
        if (root_->isFull()) {
            ResultAfterSplit res = splitIntoTwoNodes(root_);
            root_ = new Node(new int64_t[max_size_]{res.key_goes_up},
                             new Node *[max_children_number_] { res.left_child, res.right_child },
                             1, degree_, false);
        }
    }

    size_t size() const {
        if (root_ == nullptr) {
            return 0;
        }
        int64_t size = getSizeRecursive(root_);
        /*
        if (size == 39 && insert_invoke_count_ == 50) {
            throw new std::invalid_argument("");
        }
         */
        //if (equal_keys_insertion_ > 10) {
        //    throw new std::invalid_argument("");
        //}
        return size;
    }

    int64_t sum() const {
        if (root_ == nullptr) {
            return 0;
        }
        int64_t sum = getSumRecursive(root_);
        /*
        if (size() == 39 && sum > 10000000000) {
            throw new std::invalid_argument("");
        }
         */
        return sum;
    }
};

void startTests() {
    BTree b_tree(2);
    srand(time(0));
    /*
    for (int i = 1; i <= 39; ++i) {
        b_tree.insert(i);
    }
     */
    int array[5];
    int numb;
    for (int i = 0; i < 5; ++i) {
        numb = rand() % 100;
        array[i] = numb;
        b_tree.insert(numb);
    }
    b_tree.printTree();
    std::cout << b_tree.size() << "\n";
    std::cout << b_tree.sum() << "\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << array[i] << " ";
    }
}

int main() {

    startTests();

    return 0;
}