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

int upperBound(const std::vector<int> &data, int left, int right, int query) {
    left -= 1;
    right += 1;
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid] <= query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

int binaryInsertionSort(std::vector<int> *data) {
    int upper_bound;
    int insertion_element;
    int shifts_count = 0;
    for (int i = 1; i < data->size(); ++i) {
        insertion_element = (*data)[i];
        upper_bound = upperBound(*data, 0, i - 1, (*data)[i]);
        for (int j = i - 1; j >= upper_bound; --j) {
            (*data)[j + 1] = (*data)[j];
        }
        shifts_count += i - upper_bound;
        (*data)[upper_bound] = insertion_element;
    }
    return shifts_count;
}

int main() {
    std::vector<int> elements = readVectorFromConsole();
    int number_of_shifts = binaryInsertionSort(&elements);
    for (int i = 0; i < elements.size(); ++i) {
        std::cout << elements[i] << " ";
    }
    std::cout << "\n" << number_of_shifts;

    return 0;
}
