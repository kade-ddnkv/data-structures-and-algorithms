#include <iostream>

class HashTable {
    int* container_;
    int max_size_;

    int hash(int item) {
        return 1;
    }

public:
    HashTable(int max_size) {
        max_size_ = max_size;
        container_ = new int[max_size];
        for (int i = 0; i < max_size; ++i) {
            container_[i] = 0;
        }
    }
    
    void insert(int item) {
        int i = hash(item);
        while (container_[i] != 0) {
            i = (i + 1) % max_size_;
        }
        container_[i] = item;
    }

    int* search(int item) {
        int i = hash(item);
        while (container_[i] != 0) {
            if (container_[i] == item) {
                return &container_[i];
            }
            i = (i + 1) % max_size_;
        }
        return nullptr;
    }

    ~HashTable() {
        delete [] container_;
    }
};

int main() {
    HashTable t(5);
    t.insert(5);
    t.insert(6);
    t.insert(7);
    int* item = t.search(8);
    std::cout << item;
    return 0;
}
