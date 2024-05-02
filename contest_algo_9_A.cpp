#include <iostream>
#include <string>
#include <vector>
#include <queue>

std::string readString() {
    std::string input_str;
    std::cin >> input_str;
    return input_str;
}

const int kAlphabetSize = 26;
const int kAlphabetStartSymbol = 'a';

struct HuffmanTreeNode {
    int symbol;
    int weight;
    HuffmanTreeNode* left = nullptr;
    HuffmanTreeNode* right = nullptr;

    HuffmanTreeNode(int symbol, int weight) {
        this->symbol = symbol;
        this->weight = weight;
    }
};

class HuffmanTreeNodeComparer {
public:
    bool operator()(HuffmanTreeNode* lhs, HuffmanTreeNode* rhs) {
        return lhs->weight > rhs->weight;
    }
};

class HuffmanCode {
private:
    HuffmanTreeNode* root_ = nullptr;
    std::vector<std::string>* mapper_ = nullptr;

    std::vector<int> getSymbolOccurrence(std::string str) {
        std::vector<int> occurrence(kAlphabetSize);
        for (const int symbol : str) {
            ++occurrence[symbol - kAlphabetStartSymbol];
        }
        return occurrence;
    }

    void getCodesOfSymbolsRecursive(HuffmanTreeNode* node, std::string node_string,
                                    std::vector<std::string>* result_map) {
        if (node == nullptr) {
            return;
        }
        if (node->symbol != -1) {
            (*result_map)[node->symbol] = node_string;
        } else {
            getCodesOfSymbolsRecursive(node->left, node_string + '0', result_map);
            getCodesOfSymbolsRecursive(node->right, node_string + '1', result_map);
        }
    }

    void clearNodesRecursive(HuffmanTreeNode* node) {
        if (node == nullptr) {
            return;
        }
        if (node->symbol == -1) {
            clearNodesRecursive(node->left);
            clearNodesRecursive(node->right);
        }
        delete node;
    }

public:
    explicit HuffmanCode(std::string string_to_encode) {
        std::vector<int> symbol_occurrence = getSymbolOccurrence(string_to_encode);
        std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>,
                            HuffmanTreeNodeComparer>
            queue;
        for (size_t i = 0; i < symbol_occurrence.size(); ++i) {
            if (symbol_occurrence[i] != 0) {
                queue.push(new HuffmanTreeNode(i, symbol_occurrence[i]));
            }
        }

        while (queue.size() != 1) {
            HuffmanTreeNode* left = queue.top();
            queue.pop();
            HuffmanTreeNode* right = queue.top();
            queue.pop();
            HuffmanTreeNode* new_node = new HuffmanTreeNode(-1, left->weight + right->weight);
            new_node->left = left;
            new_node->right = right;
            queue.push(new_node);
        }
        root_ = queue.top();
    }

    std::vector<std::string> getCodesOfSymbols() {
        if (mapper_ != nullptr) {
            return *mapper_;
        }
        std::vector<std::string>* result_map = new std::vector<std::string>(kAlphabetSize);
        if (root_->symbol != -1) {
            (*result_map)[root_->symbol] = '0';
        } else {
            getCodesOfSymbolsRecursive(root_, "", result_map);
        }
        mapper_ = result_map;
        return *result_map;
    }

    std::string getEncoded(std::string input) {
        if (mapper_ == nullptr) {
            getCodesOfSymbols();
        }
        std::string result_str;
        for (const char symbol : input) {
            result_str += (*mapper_)[symbol - kAlphabetStartSymbol];
        }
        return result_str;
    }

    ~HuffmanCode() {
        clearNodesRecursive(root_);
        delete mapper_;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);

    // Кодирование.
    std::string input = readString();
    HuffmanCode huffman(input);
    std::string encoded = huffman.getEncoded(input);
    std::vector<std::string> mapper = huffman.getCodesOfSymbols();

    int unique_letters_count = 0;
    for (size_t i = 0; i < mapper.size(); ++i) {
        if (!mapper[i].empty()) {
            ++unique_letters_count;
        }
    }

    // Вывод.
    std::cout << unique_letters_count << " " << encoded.size() << "\n";
    for (size_t i = 0; i < mapper.size(); ++i) {
        if (!mapper[i].empty()) {
            std::cout << char(i + kAlphabetStartSymbol) << ": " << mapper[i] << "\n";
        }
    }
    std::cout << encoded;
    return 0;
}
