#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cmath>

// Дерево отрезков.
std::vector<int64_t> tree;
int leaves_count;

std::tuple<std::vector<int>, int> readNumbersFromConsole() {
    int number_of_elements;
    int window;
    std::cin >> number_of_elements >> window;

    std::vector<int> numbers(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> numbers[i];
    }
    return std::make_tuple(numbers, window);
}

int64_t buildTreeRecursive(size_t index) {
    if (index > tree.size() - leaves_count) {
        return tree[index - 1];
    }
    tree[index - 1] = std::min(buildTreeRecursive(index * 2), buildTreeRecursive(index * 2 + 1));
    return tree[index - 1];
}

void createTree(const std::vector<int>& numbers) {
    leaves_count = pow(2, log2(numbers.size() - 1) + 1);
    tree.resize(pow(2, log2(leaves_count) + 1) - 1);
    for (size_t i = tree.size() - leaves_count; i < tree.size(); ++i) {
        tree[i] = INT32_MAX;
    }
    for (size_t i = 0; i < numbers.size(); ++i) {
        tree[tree.size() - leaves_count + i] = numbers[i];
    }
    buildTreeRecursive(1);
}

int64_t query(int left, int right) {
    // left и right - это номера вершин в дереве (индекс + 1).
    int64_t result = INT32_MAX;
    while (left <= right) {
        if (left % 2 != 0) {
            result = std::min(result, tree[left - 1]);
        }
        if (right % 2 == 0) {
            result = std::min(result, tree[right - 1]);
        }
        left = (left + 1) / 2;
        right = (right - 1) / 2;
    }
    return result;
}

void printMinOfEachWindow(const std::vector<int>& numbers, int window) {
    createTree(numbers);
    for (size_t i = 0; i < numbers.size() - window + 1; ++i) {
        std::cout << query(tree.size() - leaves_count + i + 1,
                           tree.size() - leaves_count + i + window)
                  << "\n";
    }
}

int main() {
    auto[numbers, window] = readNumbersFromConsole();
    printMinOfEachWindow(numbers, window);

    return 0;
}
