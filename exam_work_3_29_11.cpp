#include <iostream>

struct Cell {
    int value;
    int x;
    int y;

    Cell(int value, int x, int y) {
    }
};

class SparseMatrix {
    Cell* data_ = new Cell[]{Cell(1, 0, 0), Cell(2, 1, 1), Cell(3, 0, 1)};
    int n_;
    int m_;

public:
    SparseMatrix(int n, int m) {
        n_ = n;
        m_ = m;
        //data_ = new int[0];
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
        delete[] container_;
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
