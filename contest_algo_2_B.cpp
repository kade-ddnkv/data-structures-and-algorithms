#include <iostream>
#include <vector>

int lowerBound(const std::vector<int> &numbers, int query) {
    int left = -1;
    int right = numbers.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (numbers[mid] < query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

int upperBound(const std::vector<int> &numbers, int query) {
    int left = -1;
    int right = numbers.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (numbers[mid] <= query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

int main() {
    int numbers_count;
    int queries_count;
    std::cin >> numbers_count >> queries_count;

    std::vector<int> numbers(numbers_count);
    for (int i = 0; i < numbers_count; ++i) {
        std::cin >> numbers[i];
    }

    int query_number;
    int lower_bound;
    int upper_bound;
    for (int i = 0; i < queries_count; ++i) {
        std::cin >> query_number;
        lower_bound = lowerBound(numbers, query_number);
        upper_bound = upperBound(numbers, query_number);
        if (lower_bound == upper_bound) {
            std::cout << 0 << "\n";
        } else {
            std::cout << lower_bound + 1 << " " << upper_bound << "\n";
        }
    }

    return 0;
}
