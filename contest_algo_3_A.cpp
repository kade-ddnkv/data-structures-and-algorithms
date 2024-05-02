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

int bubbleSortUpdated(std::vector<int> *data) {
    int upped_bound;
    int last_swap = data->size() - 1;
    int swaps_count = 0;
    do {
        upped_bound = last_swap;
        last_swap = 0;
        for (int i = 0; i < upped_bound; ++i) {
            if ((*data)[i] > (*data)[i + 1]) {
                std::swap((*data)[i], (*data)[i + 1]);
                last_swap = i;
                ++swaps_count;
            }
        }
    } while (last_swap != 0);
    return swaps_count;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    int number_of_swaps = bubbleSortUpdated(&elements);
    for (int i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << " ";
    }
    std::cout << "\n" << number_of_swaps;

    return 0;
}
