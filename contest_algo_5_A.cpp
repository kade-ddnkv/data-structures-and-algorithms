#include <iostream>
#include <vector>

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

int getIndexOfLastMaxHeapElement(const std::vector<int> &elements) {
    for (size_t i = 1; i < elements.size(); ++i) {
        if (elements[i] > elements[(i - 1) / 2]) {
            return i - 1;
        }
    }
    return elements.size() - 1;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    std::cout << getIndexOfLastMaxHeapElement(elements);
    return 0;
}
