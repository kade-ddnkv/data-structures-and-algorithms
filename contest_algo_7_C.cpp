#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::string readLine() {
    std::string line;
    getline(std::cin, line);
    return line;
}

int minFromThree(const int &num1, const int &num2, const int &num3) {
    return std::min(std::min(num1, num2), num3);
}

int getLevenshteinDistance(std::string first, std::string second) {
    std::vector<std::vector<int>> matrix(first.size() + 1);
    for (size_t i = 0; i < first.size() + 1; ++i) {
        matrix[i].resize(second.size() + 1);
    }
    for (size_t i = 0; i < first.size() + 1; ++i) {
        matrix[i][0] = i;
    }
    for (size_t i = 0; i < second.size() + 1; ++i) {
        matrix[0][i] = i;
    }
    for (size_t i = 1; i < first.size() + 1; ++i) {
        for (size_t j = 1; j < second.size() + 1; ++j) {
            matrix[i][j] =
                minFromThree(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                             matrix[i - 1][j - 1] + (first[i - 1] == second[j - 1] ? 0 : 1));
        }
    }
    return matrix[first.size()][second.size()];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << getLevenshteinDistance(readLine(), readLine());
    return 0;
}
