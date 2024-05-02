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

int selectionSort(std::vector<int> *data) {
    int swaps_count = 0;
    int local_min_index;
    for (int i = 0; i < static_cast<int>(data->size()) - 1; ++i) {
        local_min_index = i;
        for (int j = i + 1; j < data->size(); ++j) {
            if ((*data)[j] < (*data)[local_min_index]) {
                local_min_index = j;
            }
        }
        if (i != local_min_index) {
            std::swap((*data)[i], (*data)[local_min_index]);
            ++swaps_count;
        }
    }
    return swaps_count;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    int number_of_swaps = selectionSort(&elements);
    for (int i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << " ";
    }
    std::cout << "\n" << number_of_swaps;

    return 0;
}
