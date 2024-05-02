#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

std::tuple<int, int> partitionByMiddle(std::vector<int> *list, int start, int end) {
    int pivot_value = (*list)[start + (end - start) / 2];
    int left = start;
    int right = end;
    while (left <= right) {
        while ((*list)[left] < pivot_value) {
            ++left;
        }
        while ((*list)[right] > pivot_value) {
            --right;
        }
        if (left >= right) {
            return {left, right};
        }
        std::swap((*list)[left++], (*list)[right--]);
    }
    return {left, right};
}

void quickSortRecursive(std::vector<int> *list, int start, int end, int *recursive_calls_count) {
    if (start < end) {
        if (start != 0 || end != static_cast<int>(list->size()) - 1) {
            ++(*recursive_calls_count);
        }
        auto[pivot_left, pivot_right] = partitionByMiddle(list, start, end);
        if (pivot_left == pivot_right) {
            quickSortRecursive(list, start, pivot_right - 1, recursive_calls_count);
        } else {
            quickSortRecursive(list, start, pivot_right, recursive_calls_count);
        }
        quickSortRecursive(list, pivot_right + 1, end, recursive_calls_count);
    }
}

int quickSort(std::vector<int> *list) {
    int recursive_calls_count = 0;
    quickSortRecursive(list, 0, list->size() - 1, &recursive_calls_count);
    return recursive_calls_count;
}

int main() {
    std::vector<int> list = readVectorFromConsole();
    std::cout << quickSort(&list) << "\n";
    for (const int &element : list) {
        std::cout << element << " ";
    }
    return 0;
}
