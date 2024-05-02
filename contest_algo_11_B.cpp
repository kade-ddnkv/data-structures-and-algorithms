#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

std::string getLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int getMaxLengthOfSubsequence(std::string first, std::string second) {
    // Инициализация двумерного массива.
    std::vector<std::vector<int>> subtasks(first.length() + 1);
    for (size_t i = 0; i <= first.length(); ++i) {
        subtasks[i].resize(second.length() + 1);
    }
    for (size_t i = 0; i <= first.length(); ++i) {
        subtasks[i][0] = 0;
    }
    for (size_t i = 0; i <= second.length(); ++i) {
        subtasks[0][i] = 0;
    }

    // Заполнение.
    for (size_t i = 1; i <= first.length(); ++i) {
        for (size_t j = 1; j <= second.length(); ++j) {
            if (first[i - 1] == second[j - 1]) {
                subtasks[i][j] = subtasks[i - 1][j - 1] + 1;
            } else {
                subtasks[i][j] = std::max(subtasks[i - 1][j], subtasks[i][j - 1]);
            }
        }
    }
    return subtasks[first.length()][second.length()];
}

int main() {
    std::string first = getLine();
    std::string second = getLine();
    std::cout << getMaxLengthOfSubsequence(first, second);
    return 0;
}
