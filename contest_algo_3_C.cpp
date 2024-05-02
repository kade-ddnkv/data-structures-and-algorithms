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

int simpleInsertionSort(std::vector<int> *data) {
    int already_sorted_count = 0;
    int local_swaps_count = 0;
    for (int i = 1; i < data->size(); ++i) {
        local_swaps_count = 0;
        for (int j = i - 1; j >= 0 && (*data)[j] > (*data)[j + 1]; --j) {
            std::swap((*data)[j], (*data)[j + 1]);
            ++local_swaps_count;
        }
        if (local_swaps_count == 0) {
            ++already_sorted_count;
        }
    }
    return already_sorted_count;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    int already_sorted = simpleInsertionSort(&elements);
    for (int i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << " ";
    }
    std::cout << "\n" << already_sorted;

    return 0;
}
