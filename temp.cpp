#include <iostream>
#include <string>

class TreeFromArray {
    struct Node {
        enum Color { RED, BLACK };
        int key_;
        int index_of_left_;
        int index_of_right_;
        Color color_;

        void set(int key, int index_of_left, int index_of_right, char color) {
            key_ = key;
            index_of_left_ = index_of_left;
            index_of_right_ = index_of_right;
            color_ = color == 'R' ? RED : BLACK;
        }
    };

    Node* nodes_;
    int number_of_nodes_;
    int index_of_root_;

    bool isLessLeftRecursive(const Node& root, int curr_index) {
        if (curr_index == -1) {
            return true;
        }
        const Node& curr_node = nodes_[curr_index];
        if (curr_node.key_ < root.key_) {
            return isLessLeftRecursive(root, curr_node.index_of_left_);
        } else {
            return false;
        }
    }

    bool isMoreRightRecursive(const Node& root, int curr_index) {
        if (curr_index == -1) {
            return true;
        }
        const Node& curr_node = nodes_[curr_index];
        if (curr_node.key_ > root.key_) {
            return isMoreRightRecursive(root, curr_node.index_of_right_);
        } else {
            return false;
        }
    }

    bool isBinarySearchTree() {
        for (int i = 0; i < number_of_nodes_; ++i) {
            if (!isLessLeftRecursive(nodes_[i], nodes_[i].index_of_left_) ||
                !isMoreRightRecursive(nodes_[i], nodes_[i].index_of_right_)) {
                return false;
            }
        }
        return true;
    }

    bool colorsAreCorrect(int curr_index) {
        if (curr_index == -1) {
            return true;
        }
        Node current_node = nodes_[curr_index];
        if (current_node.color_ == Node::BLACK ||
            (current_node.color_ == Node::RED &&
             (current_node.index_of_left_ == -1 ||
              nodes_[current_node.index_of_left_].color_ == Node::BLACK) &&
             (current_node.index_of_right_ == -1 ||
              nodes_[current_node.index_of_right_].color_ == Node::BLACK))) {
            return colorsAreCorrect(current_node.index_of_left_) &&
                   colorsAreCorrect(current_node.index_of_right_);
        } else {
            return false;
        }
    }

    int getBlackHeight(int curr_index, bool* all_black_paths_are_equal) {
        if (curr_index == -1) {
            return 1;
        }
        const Node& curr_node = nodes_[curr_index];
        int left_height = getBlackHeight(curr_node.index_of_left_, all_black_paths_are_equal);
        if (!(*all_black_paths_are_equal)) {
            return 0;
        }
        int right_height = getBlackHeight(curr_node.index_of_right_, all_black_paths_are_equal);
        if (left_height != right_height) {
            *all_black_paths_are_equal = false;
        }
        return left_height + (curr_node.color_ == Node::BLACK ? 1 : 0);
    }

    bool allBlackPathsAreEqual() {
        bool* true_for_curr_node = new bool();
        for (int i = 0; i < number_of_nodes_; ++i) {
            *true_for_curr_node = true;
            getBlackHeight(i, true_for_curr_node);
            if (!(*true_for_curr_node)) {
                delete true_for_curr_node;
                return false;
            }
        }
        delete true_for_curr_node;
        return true;
    }

public:
    explicit TreeFromArray(int number_of_nodes, int index_of_root) {
        number_of_nodes_ = number_of_nodes;
        index_of_root_ = index_of_root;
        nodes_ = new Node[number_of_nodes];
    }

    void readFromConsole() {
        int number;
        int key;
        std::string left;
        std::string right;
        char color;
        for (int i = 0; i < number_of_nodes_; ++i) {
            std::cin >> number >> key >> left >> right >> color;
            nodes_[number - 1].set(key, left == "null" ? -1 : std::stoi(left) - 1,
                                   right == "null" ? -1 : std::stoi(right) - 1, color);
        }
    }

    bool isRedBlackTree() {
        if (!isBinarySearchTree() || nodes_[index_of_root_].color_ != Node::BLACK ||
            !colorsAreCorrect(index_of_root_) || !allBlackPathsAreEqual()) {
            return false;
        }
        return true;
    }

    ~TreeFromArray() {
        delete[] nodes_;
    }
};

int main() {
    int number_of_nodes;
    int number_of_root;
    std::cin >> number_of_nodes;
    if (number_of_nodes == 0) {
        std::cout << "YES";
        return 0;
    }
    std::cin >> number_of_root;

    TreeFromArray tree = TreeFromArray(number_of_nodes, number_of_root - 1);
    tree.readFromConsole();
    std::cout << (tree.isRedBlackTree() ? "YES" : "NO");

    return 0;
}
