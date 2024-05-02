#include <iostream>
#include <initializer_list>
#include <vector>
#include <utility>

template <class ValueType, class = std::enable_if_t<!std::is_pointer_v<ValueType>>>
class Heap {
private:
    std::vector<ValueType> vector_;

    void siftDown(std::vector<ValueType> *data, int root_index, int right_border) {
        int root = root_index;
        int left, right, max;
        while (root * 2 + 1 <= right_border) {
            left = root * 2 + 1;
            right = root * 2 + 2;
            max = root;
            if ((*data)[max] < (*data)[left]) {
                max = left;
            }
            if (right <= right_border && (*data)[max] < (*data)[right]) {
                max = right;
            }
            if (max == root) {
                return;
            }
            std::swap((*data)[root], (*data)[max]);
            root = max;
        }
    }

    void siftUp(std::vector<ValueType> *data, int root_index) {
        int root = root_index;
        int parent;
        while (root > 0) {
            parent = (root - 1) / 2;
            if ((*data)[parent] < (*data)[root]) {
                std::swap((*data)[parent], (*data)[root]);
                root = parent;
            } else {
                return;
            }
        }
    }

    void makeHeap(std::vector<ValueType> *data) {
        for (int i = (data->size() - 1 - 1) / 2; i >= 0; --i) {
            siftDown(data, i, data->size() - 1);
        }
    }

public:
    Heap() {
    }

    template <class Iterator>
    Heap(Iterator begin, Iterator end) {
        Iterator curr = begin;
        while (curr != end) {
            vector_.push_back(*curr);
            ++curr;
        }
        makeHeap(&vector_);
    }

    Heap(std::initializer_list<ValueType> init_list) : Heap(init_list.begin(), init_list.end()) {
    }

    Heap(const Heap &source) : vector_(source.vector_) {
    }

    Heap &operator=(const Heap &source) {
        vector_ = source.vector_;
        return *this;
    }

    Heap(Heap &&source) : vector_(source.vector_) {
        source.vector_.clear();
    }

    Heap &operator=(Heap &&source) {
        vector_ = source.vector_;
        source.vector_.clear();
        return *this;
    }

    ~Heap() {
    }

    size_t size() const {
        return vector_.size();
    }

    bool empty() const {
        return vector_.empty();
    }

    void insert(const ValueType &new_value) {
        vector_.push_back(new_value);
        siftUp(&vector_, vector_.size() - 1);
    }

    ValueType extract() {
        ValueType extracted = vector_[0];
        std::swap(vector_[0], vector_[vector_.size() - 1]);
        vector_.pop_back();
        siftDown(&vector_, 0, vector_.size() - 1);
        return extracted;
    }
};
