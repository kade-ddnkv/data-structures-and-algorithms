#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> readElementsFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

int getMaxSubsequenceLength(std::vector<int> elements) {
    // Вектор элементов копируется, чтобы не менять исходную последовательность.
    std::sort(elements.begin(), elements.end());
    std::unique(elements.begin(), elements.end());

    int curr_sequence_length = 1;
    int max_sequence_length = 1;
    for (size_t i = 1; i < elements.size(); ++i) {
        if (elements[i] - elements[i - 1] == 1) {
            curr_sequence_length += 1;
        } else {
            if (curr_sequence_length > max_sequence_length) {
                max_sequence_length = curr_sequence_length;
            }
            curr_sequence_length = 1;
        }
    }
    if (curr_sequence_length > max_sequence_length) {
        max_sequence_length = curr_sequence_length;
    }
    return max_sequence_length;
}

int main() {
    auto elements = readElementsFromConsole();

    auto max_sequence_length = getMaxSubsequenceLength(elements);

    std::cout << max_sequence_length;

    return 0;
}
