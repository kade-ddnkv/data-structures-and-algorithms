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

int partitionByLast(std::vector<int> *list, int start, int end) {
    int pivot_value = (*list)[end];
    int pivot_future_index = start;
    int index = start;
    while (index < end) {
        if ((*list)[index] <= pivot_value) {
            std::swap((*list)[index], (*list)[pivot_future_index]);
            ++pivot_future_index;
        }
        ++index;
    }
    std::swap((*list)[pivot_future_index], (*list)[end]);
    return pivot_future_index;
}

void quickSortRecursive(std::vector<int> *list, int start, int end, int *recursive_calls_count) {
    if (start < end) {
        if (start != 0 || end != static_cast<int>(list->size()) - 1) {
            ++(*recursive_calls_count);
        }
        int pivot = partitionByLast(list, start, end);
        quickSortRecursive(list, start, pivot - 1, recursive_calls_count);
        quickSortRecursive(list, pivot + 1, end, recursive_calls_count);
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
