#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <utility>

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> readGems() {
    int red_size, green_size, blue_size;
    std::cin >> red_size >> green_size >> blue_size;
    std::vector<int> red(red_size);
    std::vector<int> green(green_size);
    std::vector<int> blue(blue_size);
    for (size_t i = 0; i < red.size(); ++i) {
        std::cin >> red[i];
    }
    for (size_t i = 0; i < green.size(); ++i) {
        std::cin >> green[i];
    }
    for (size_t i = 0; i < blue.size(); ++i) {
        std::cin >> blue[i];
    }
    return {red, green, blue};
}

std::tuple<int, int> partitionByMiddle(std::vector<int> *list, int start, int end) {
    int pivot_value = (*list)[start + (end - start) / 2];
    int left = start;
    int right = end;
    while (left <= right) {
        while ((*list)[left] < pivot_value) {
            ++left;
        }
        while ((*list)[right] > pivot_value) {
            --right;
        }
        if (left >= right) {
            return {left, right};
        }
        std::swap((*list)[left++], (*list)[right--]);
    }
    return {left, right};
}

void quickSortRecursive(std::vector<int> *list, int start, int end, int *recursive_calls_count) {
    if (start < end) {
        if (start != 0 || end != static_cast<int>(list->size()) - 1) {
            ++(*recursive_calls_count);
        }
        auto[pivot_left, pivot_right] = partitionByMiddle(list, start, end);
        if (pivot_left == pivot_right) {
            quickSortRecursive(list, start, pivot_right - 1, recursive_calls_count);
        } else {
            quickSortRecursive(list, start, pivot_right, recursive_calls_count);
        }
        quickSortRecursive(list, pivot_right + 1, end, recursive_calls_count);
    }
}

int quickSort(std::vector<int> *list) {
    int recursive_calls_count = 0;
    quickSortRecursive(list, 0, list->size() - 1, &recursive_calls_count);
    return recursive_calls_count;
}

int upperBoundInclusive(const std::vector<int> &data, int query) {
    int left = -1;
    int right = data.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid] < query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

int lowerBoundInclusive(const std::vector<int> &data, int query) {
    int left = -1;
    int right = data.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid] <= query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return left;
}

void printThreeBestGems(std::vector<int> red_gems, std::vector<int> green_gems,
                        std::vector<int> blue_gems) {
    red_gems.push_back(-INT32_MAX);
    red_gems.push_back(INT32_MAX);
    green_gems.push_back(-INT32_MAX);
    green_gems.push_back(INT32_MAX);
    blue_gems.push_back(-INT32_MAX);
    blue_gems.push_back(INT32_MAX);
    quickSort(&red_gems);
    quickSort(&green_gems);
    quickSort(&blue_gems);

    int64_t min_weight_diff = INT64_MAX;
    int red_result, green_result, blue_result;
    int64_t curr_weight_diff;

    auto weight_difference_check = [&](const int &red, const int &green, const int &blue) {
        if (red == INT32_MAX || red == -INT32_MAX || blue == INT32_MAX || blue == -INT32_MAX ||
            green == INT32_MAX || green == -INT32_MAX) {
            return;
        }
        curr_weight_diff =
            pow((1LL * red - green), 2) + pow((1LL * green - blue), 2) + pow((1LL * red - blue), 2);
        if (curr_weight_diff < min_weight_diff) {
            min_weight_diff = curr_weight_diff;
            red_result = red;
            green_result = green;
            blue_result = blue;
        }
    };

    for (int i = 1; i < static_cast<int>(red_gems.size()) - 1; ++i) {
        weight_difference_check(red_gems[i],
                                green_gems[lowerBoundInclusive(green_gems, red_gems[i])],
                                blue_gems[upperBoundInclusive(blue_gems, red_gems[i])]);
        weight_difference_check(red_gems[i],
                                green_gems[upperBoundInclusive(green_gems, red_gems[i])],
                                blue_gems[lowerBoundInclusive(blue_gems, red_gems[i])]);
    }
    for (int i = 1; i < static_cast<int>(green_gems.size()) - 1; ++i) {
        weight_difference_check(red_gems[lowerBoundInclusive(red_gems, green_gems[i])],
                                green_gems[i],
                                blue_gems[upperBoundInclusive(blue_gems, green_gems[i])]);
        weight_difference_check(red_gems[upperBoundInclusive(red_gems, green_gems[i])],
                                green_gems[i],
                                blue_gems[lowerBoundInclusive(blue_gems, green_gems[i])]);
    }
    for (int i = 1; i < static_cast<int>(blue_gems.size()) - 1; ++i) {
        weight_difference_check(red_gems[lowerBoundInclusive(red_gems, blue_gems[i])],
                                green_gems[upperBoundInclusive(green_gems, blue_gems[i])],
                                blue_gems[i]);
        weight_difference_check(red_gems[upperBoundInclusive(red_gems, blue_gems[i])],
                                green_gems[lowerBoundInclusive(green_gems, blue_gems[i])],
                                blue_gems[i]);
    }

    std::cout << red_result << " " << green_result << " " << blue_result << "\n";
}

int main() {
    int number_of_subtasks;
    std::cin >> number_of_subtasks;
    for (int i = 0; i < number_of_subtasks; ++i) {
        auto[red, green, blue] = readGems();
        printThreeBestGems(red, green, blue);
    }
    return 0;
}
