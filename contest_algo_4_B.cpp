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

int getDigit(int number, int digit_place) {
    int mask = 255;
    return mask & (number >> (digit_place * 8));
}

std::vector<int> tmp_elements_copy;

void msdRadixSort(std::vector<int> *data, int left, int right, int digit_place) {
    if (digit_place < 0 || left >= right) {
        return;
    }
    int curr_digit;
    int max_digit = -1;
    for (int i = left; i <= right; ++i) {
        curr_digit = getDigit((*data)[i], digit_place);
        if (curr_digit > max_digit) {
            max_digit = curr_digit;
        }
    }
    std::vector<int> counts(max_digit + 1);
    for (int i = left; i <= right; ++i) {
        curr_digit = getDigit((*data)[i], digit_place);
        ++counts[curr_digit];
    }
    std::vector<int> recursive_calls;
    recursive_calls.push_back(counts[0]);
    for (size_t i = 1; i < counts.size(); ++i) {
        counts[i] += counts[i - 1];
        if (counts[i] != counts[i - 1]) {
            recursive_calls.push_back(counts[i]);
        }
    }
    for (int i = right; i >= left; --i) {
        curr_digit = getDigit((*data)[i], digit_place);
        --counts[curr_digit];
        tmp_elements_copy[left + counts[curr_digit]] = (*data)[i];
    }
    for (int i = left; i <= right; ++i) {
        (*data)[i] = tmp_elements_copy[i];
    }
    msdRadixSort(data, left, left + recursive_calls[0] - 1, digit_place - 1);
    for (size_t i = 1; i < recursive_calls.size(); ++i) {
        msdRadixSort(data, left + recursive_calls[i - 1], left + recursive_calls[i] - 1,
                     digit_place - 1);
    }
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    tmp_elements_copy.resize(elements.size());
    msdRadixSort(&elements, 0, elements.size() - 1, 3);
    for (const int &element : elements) {
        std::cout << element << " ";
    }
    return 0;
}
