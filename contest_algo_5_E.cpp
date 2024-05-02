#include <iostream>
#include <vector>
#include <string>
#include <utility>

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

int upperBoundInclusive(const std::vector<int> &data, int query) {
    int left = -1;
    int right = data.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid] <= query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return left;
}

int lowerBoundInclusive(const std::vector<int> &data, int query) {
    int left = -1;
    int right = data.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid] < query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return left;
}

void handleQueries(std::vector<int> *begs, std::vector<int> *ends) {
    std::string query_char;
    int query_number;
    while ((std::cin >> query_char) && query_char != "!") {
        std::cin >> query_number;
        // Количество уже начатых интервалов минус количество еще не законченных интервалов...
        // ...равно количество интервалов, которые попадают на момент запроса.
        std::cout << "! "
                  << upperBoundInclusive(*begs, query_number) -
                         lowerBoundExclusive(*ends, query_number)
                  << "\n";
        std::cout.flush();
    }
}

int main() {
    int number_of_intervals;
    std::cin >> number_of_intervals;
    std::vector<int> begs(number_of_intervals);
    std::vector<int> ends(number_of_intervals);
    for (int i = 0; i < number_of_intervals; ++i) {
        std::cin >> begs[i] >> ends[i];
    }
    heapSort(&begs);
    heapSort(&ends);
    handleQueries(&begs, &ends);
    return 0;
}
