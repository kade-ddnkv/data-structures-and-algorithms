#include <iostream>

// Элемент хеш-таблицы - список объектов с одним хешем
template <class KeyType, class ValueType>
struct Node {
    KeyType key_;
    ValueType value_;
    Node *next_ = nullptr;

    Node(KeyType key, ValueType value) {
        key_ = key;
        value_ = value;
    }
};

// Хеш-таблица
template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
    Node<KeyType, ValueType> **table_;
    size_t cap_;
    size_t size_ = 0;
    double fullness_point_;
    Func hasher_;

    Node<KeyType, ValueType> **initializeTable(size_t cap) {
        auto table = new Node<KeyType, ValueType> *[cap];
        for (size_t i = 0; i < cap; ++i) {
            table[i] = nullptr;
        }
        return table;
    }

    void insertNewValueInTable(Node<KeyType, ValueType> **table, size_t hash, KeyType key,
                               ValueType value, bool virtual_table) {
        if (table[hash] == nullptr) {
            table[hash] = new Node<KeyType, ValueType>(key, value);
        } else {
            Node<KeyType, ValueType> *curr = table[hash];
            while (curr->next_ != nullptr) {
                curr = curr->next_;
            }
            curr->next_ = new Node<KeyType, ValueType>(key, value);
        }
        if (!virtual_table) {
            ++size_;
            if (static_cast<double>(size_) / cap_ > fullness_point_) {
                doubleCapacityAndRehash();
            }
        }
    }

    void doubleCapacityAndRehash() {
        // std::cout << "doubling!\n";

        size_t new_cap;
        if (cap_ == 0) {
            new_cap = 1;
        } else {
            new_cap = cap_ * 2;
        }
        auto new_table = initializeTable(new_cap);

        // Перенос значений из прошлой таблицы в новую.
        for (size_t i = 0; i < cap_; ++i) {
            Node<KeyType, ValueType> *curr = table_[i];
            while (curr != nullptr) {
                size_t hash = hasher_(curr->key_) % new_cap;
                insertNewValueInTable(new_table, hash, curr->key_, curr->value_, true);
                curr = curr->next_;
            }
        }

        clear();
        cap_ = new_cap;
        table_ = new_table;
    }

    void clear() {
        for (size_t i = 0; i < cap_; ++i) {
            if (table_[i] != nullptr) {
                Node<KeyType, ValueType> *curr = table_[i];
                Node<KeyType, ValueType> *prev = nullptr;
                while (curr != nullptr) {
                    prev = curr;
                    curr = curr->next_;
                    delete prev;
                }
            }
        }
        delete[] table_;
    }

public:
    HashTable() {
        cap_ = 100;
        fullness_point_ = 0.5;
        hasher_ = Func();
        table_ = initializeTable(cap_);
    }

    explicit HashTable(Func hasher) {
        cap_ = 100;
        fullness_point_ = 0.5;
        hasher_ = hasher;
        table_ = initializeTable(cap_);
    }

    HashTable(size_t cap, double fullness_point, Func hasher = std::hash<KeyType>()) {
        cap_ = cap;
        if (fullness_point <= 0 || fullness_point > 1) {
            fullness_point_ = 0.5;
        } else {
            fullness_point_ = fullness_point;
        }
        hasher_ = hasher;
        table_ = initializeTable(cap_);
    }

    ~HashTable() {
        clear();
    }

    void insert(KeyType key, ValueType value) {
        if (cap_ == 0) {
            doubleCapacityAndRehash();
            size_t hash = hasher_(key) % cap_;
            insertNewValueInTable(table_, hash, key, value, false);
            return;
        }
        if (find(key)) {
            size_t hash = hasher_(key) % cap_;
            (*table_[hash]).value_ = value;
        } else {
            size_t hash = hasher_(key) % cap_;
            insertNewValueInTable(table_, hash, key, value, false);
        }
    }

    ValueType *find(KeyType key) {
        if (cap_ == 0) {
            return nullptr;
        }
        size_t hash = hasher_(key) % cap_;
        if (table_[hash] != nullptr) {
            Node<KeyType, ValueType> *curr = table_[hash];
            while (curr != nullptr) {
                if (curr->key_ == key) {
                    return &curr->value_;
                }
                curr = curr->next_;
            }
        }
        return nullptr;
    }

    void erase(KeyType key) {
        if (cap_ == 0) {
            return;
        }
        size_t hash = hasher_(key) % cap_;
        if (table_[hash] != nullptr) {
            Node<KeyType, ValueType> *curr = table_[hash];
            Node<KeyType, ValueType> *prev = nullptr;
            while (curr != nullptr) {
                if (curr->key_ == key) {
                    if (prev != nullptr) {
                        prev->next_ = curr->next_;
                    } else {
                        table_[hash] = curr->next_;
                    }
                    delete curr;
                    --size_;
                    return;
                }
                prev = curr;
                curr = curr->next_;
            }
        }
    }

    Node<KeyType, ValueType> &operator[](uint64_t hash) {
        if (hash < 0 || hash > cap_ - 1) {
            throw std::out_of_range("");
        }
        if (table_[hash] == nullptr) {
            throw std::runtime_error("");
        }
        return *table_[hash];
    }

    Node<KeyType, ValueType> at(uint64_t hash) {
        if (hash < 0 || hash > cap_ - 1) {
            throw std::out_of_range("");
        }
        if (table_[hash] == nullptr) {
            throw std::runtime_error("");
        }
        return *table_[hash];
    }

    size_t size() const {
        // size_t size = 0;
        // for (size_t i = 0; i < cap_; ++i) {
        //     size += nodeSize(table_[i]);
        // }
        // return size;
        return size_;
    }

    size_t capacity() const {
        return cap_;
    }
};

int main() {
    HashTable<std::string, int> table;

    for (int i = 0; i < 50; ++i) {
        table.insert(std::to_string(i), i);
    }
    {
        std::cout << table[62].next_->value_ << "\n";
        std::cout << table.at(62).value_ << "\n";
    }
    /*
    for (int i = 0; i < 52; ++i) {
        table.erase(std::to_string(i));
    }
     */
    table.erase("14");
    std::cout << table[62].next_ << "\n";
    std::cout << table.size() << "\n";
    std::cout << table.capacity() << "\n";
    size_t count = 0;
    for (size_t i = 0; i < table.capacity(); ++i) {
        if (table.find(std::to_string(i)) != nullptr) {
            ++count;
            // throw new std::out_of_range("");
        }
    }
    std::cout << count;  // Должно быть 51, т.к. нет коллизий.
    /*
    std::cout << table.size() << "\n";
    std::cout << table.find(1) << "\n";
    table.insert(1, 11);
    // table.insert(1, 12);
    table.insert(101, 12);
    table.insert(2, 22);
    std::cout << table.size() << "\n";
    table.insert(3, 33);
    table.insert(3, 34);

    std::cout << table.size() << "\n";
    table.erase(2);
    std::cout << table.size() << "\n";
    // std::cout << table.at(1).next_->value_ << "\n";
    std::cout << *table.find(1) << "\n";
    std::cout << *table.find(101) << "\n";
    std::cout << *table.find(3) << "\n";
     */

    /*
    table.insert("a", 1);
    table.insert("b", 1);
    table.insert("c", 1);
    table.insert("d", 1);
    table.insert("e", 1);
    table.insert("f", 1);


    table.insert("1", 11);
    table.insert("101", 22);
    table.insert("1001", 33);
    table.insert("10001", 44);
    table.insert("100001", 55);
    table.insert("1000001", 66);

    std::cout << *table.find("1") << "\n";
    // std::cout << *table.find(2) << "\n";
    // std::cout << *table.find(3) << "\n";
    // std::cout << *table.find(4) << "\n";
    // std::cout << *table.find(5) << "\n";
    // std::cout << *table.find(101) << "\n";
     */

    return 0;
}