#include <iostream>

template <typename T>
class Bst {
    struct Node {
        T value;
        Node *left = nullptr;
        Node *right = nullptr;

        explicit Node(T value) {
            this->value = value;
        }

        ~Node() {
            delete left;
            delete right;
        }
    };

    Node *root_ = nullptr;

    void printLeavesWhileInOrderTraversal(Node *root) {
        if (root) {
            printLeavesWhileInOrderTraversal(root->left);
            if (root->left == nullptr && root->right == nullptr) {
                std::cout << root->value << "\n";
            }
            printLeavesWhileInOrderTraversal(root->right);
        }
    }

public:
    void insert(T value) {
        if (root_ == nullptr) {
            root_ = new Node(value);
            return;
        }

        Node *curr_node = root_;
        bool node_inserted = false;
        while (!node_inserted) {
            if (value < curr_node->value) {
                if (curr_node->left == nullptr) {
                    curr_node->left = new Node(value);
                    node_inserted = true;
                } else {
                    curr_node = curr_node->left;
                }
            } else if (value > curr_node->value) {
                if (curr_node->right == nullptr) {
                    curr_node->right = new Node(value);
                    node_inserted = true;
                } else {
                    curr_node = curr_node->right;
                }
            } else {
                node_inserted = true;
            }
        }
    }

    void printLeavesInAscendingOrder() {
        printLeavesWhileInOrderTraversal(root_);
    }

    ~Bst() {
        delete root_;
    }
};

Bst<int> createIntBstFromConsole() {
    Bst<int> bst;
    int value;
    while (std::cin >> value && value != 0) {
        bst.insert(value);
    }
    return bst;
}

int main() {
    Bst<int> bst = createIntBstFromConsole();
    bst.printLeavesInAscendingOrder();

    return 0;
}
