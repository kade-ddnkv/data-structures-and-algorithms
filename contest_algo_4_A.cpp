#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

std::vector<int> stableCountingSort(std::vector<int> unsorted) {
    std::vector<int> counts(*std::max_element(unsorted.begin(), unsorted.end()) + 1);
    for (size_t i = 0; i < unsorted.size(); ++i) {
        ++counts[unsorted[i]];
    }
    for (size_t i = 1; i < counts.size(); ++i) {
        counts[i] += counts[i - 1];
    }
    std::vector<int> sorted(unsorted.size());
    for (int i = unsorted.size() - 1; i >= 0; --i) {
        --counts[unsorted[i]];
        sorted[counts[unsorted[i]]] = unsorted[i];
    }
    return sorted;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    std::vector<int> sorted_elements = stableCountingSort(elements);
    for (const int &element : sorted_elements) {
        std::cout << element << " ";
    }
    return 0;
}
