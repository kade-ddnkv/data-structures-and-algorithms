#include <iostream>
#include <string>

struct TrieTreeNode {
    static const int kAlphabetSymbolsNumber = 256;
    TrieTreeNode** children;
    // edges_ - символы, привязанные к соответствующим ребрам.
    char* edges;
    int children_number;

    TrieTreeNode() {
        children = new TrieTreeNode*[kAlphabetSymbolsNumber];
        for (int i = 0; i < kAlphabetSymbolsNumber; ++i) {
            children[i] = nullptr;
        }
        edges = new char[kAlphabetSymbolsNumber];
        children_number = 0;
    }

    // Возвращает потомка, в которого ведет указанный символ.
    // Возвращает -1, если такого потомка нет.
    int getCorrespondingChildIndex(char symbol) {
        for (int i = 0; i < children_number; ++i) {
            if (edges[i] == symbol) {
                return i;
            }
        }
        return -1;
    }

    // Добавляет нового потомка (и соответствующий символ в массив edges_).
    int addChild(char symbol) {
        edges[children_number] = symbol;
        children[children_number] = new TrieTreeNode();
        ++children_number;
        return children_number - 1;
    }

    ~TrieTreeNode() {
        for (int i = 0; i < children_number; ++i) {
            delete children[i];
        }
        delete[] children;
        delete[] edges;
    }
};

class TrieTree {
    TrieTreeNode* root_ = nullptr;

public:
    TrieTree() {
        root_ = new TrieTreeNode();
    }

    ~TrieTree() {
        delete root_;
    }

    // Добавляет новую строку в дерево.
    void add(const std::string& obj) {
        TrieTreeNode* curr_node = root_;
        for (char symbol : obj) {
            int child_index = curr_node->getCorrespondingChildIndex(symbol);
            if (child_index == -1) {
                child_index = curr_node->addChild(symbol);
            }
            curr_node = curr_node->children[child_index];
        }
    }

    // Определяет, хранится ли строка в дереве.
    bool contains(const std::string& obj) const {
        TrieTreeNode* curr_node = root_;
        for (char symbol : obj) {
            int child_index = curr_node->getCorrespondingChildIndex(symbol);
            if (child_index == -1) {
                return false;
            }
            curr_node = curr_node->children[child_index];
        }
        return true;
    }
};

// основной класс фильтра
class BloomFilter {
    size_t hashes_number_;
    size_t bits_number_;

    // Дерево для хранения настоящих введенных значений.
    TrieTree storage_;
    // Хранение массива булевских значений в памяти оптимизировано.
    bool* bits_;

    std::hash<std::string> hasher_;

    size_t verifies_number_ = 0;
    size_t incorrect_verifies_number_ = 0;

public:
    BloomFilter(size_t hashes, size_t bits) {
        hashes_number_ = hashes;
        bits_number_ = bits;
        bits_ = new bool[bits_number_];
        for (size_t i = 0; i < bits_number_; ++i) {
            bits_[i] = false;
        }
    }

    ~BloomFilter() {
        delete[] bits_;
    }

    // вставка информации об объекте
    void add(const std::string& obj) {
        bits_[hasher_(obj) % bits_number_] = true;
        for (size_t i = 1; i < hashes_number_; ++i) {
            bits_[hasher_(obj + std::to_string(i)) % bits_number_] = true;
        }
        storage_.add(obj);
    }

    // проверка принадлежности объекта
    bool verify(const std::string& obj) {
        ++verifies_number_;
        if (!bits_[hasher_(obj) % bits_number_]) {
            return false;
        }
        for (size_t i = 1; i < hashes_number_; ++i) {
            auto a = hasher_(obj + std::to_string(i)) % bits_number_;
            if (!bits_[a]) {
                return false;
            }
        }
        if (!storage_.contains(obj)) {
            ++incorrect_verifies_number_;
        }
        return true;
    }

    // считаем относительную величину false positive rate по запросам тестирования
    // принадлежности
    double getFPRate() const {
        if (verifies_number_ == 0) {
            return 0;
        }
        return incorrect_verifies_number_ / static_cast<double>(verifies_number_);
    }

    size_t numberOfHashFunctions() const {
        return hashes_number_;
    }

    size_t numberOfBits() const {
        return bits_number_;
    }
};
