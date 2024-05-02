#include <iostream>
#include <vector>
#include <utility>

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

void siftDown(std::vector<int> *data, int root_index, int right_border) {
    int root = root_index;
    int left, right, max;
    while (root * 2 + 1 <= right_border) {
        left = root * 2 + 1;
        right = root * 2 + 2;
        max = root;
        if ((*data)[left] > (*data)[max]) {
            max = left;
        }
        if (right <= right_border && (*data)[right] > (*data)[max]) {
            max = right;
        }
        if (max == root) {
            // Просеивание вниз закончено, элемент на своем месте.
            return;
        }
        std::swap((*data)[root], (*data)[max]);
        root = max;
    }
}

void makeHeap(std::vector<int> *data) {
    for (int i = (data->size() - 1 - 1) / 2; i >= 0; --i) {
        siftDown(data, i, data->size() - 1);
    }
}

void heapSort(std::vector<int> *data) {
    makeHeap(data);
    int heap_size = data->size();
    while (heap_size != 0) {
        std::swap((*data)[0], (*data)[heap_size - 1]);
        --heap_size;
        siftDown(data, 0, heap_size - 1);
    }
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    heapSort(&elements);
    for (const int &element : elements) {
        std::cout << element << " ";
    }
    return 0;
}
