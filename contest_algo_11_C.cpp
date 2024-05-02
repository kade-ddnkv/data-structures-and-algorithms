#include <iostream>
#include <vector>
#include <algorithm>

void readData(size_t *number_of_rocks, size_t *max_weight, std::vector<size_t> *rocks) {
    std::cin >> (*number_of_rocks) >> (*max_weight);
    rocks->resize(*number_of_rocks);
    for (size_t i = 0; i < (*number_of_rocks); ++i) {
        std::cin >> (*rocks)[i];
    }
}

void printRocksSequenceUnderMaxWeight(size_t max_weight, const std::vector<size_t> &rocks) {
    // Инициализация двумерного массива.
    // По горизонтали - максимальный вес.
    // По вертикали - количество камней, берущихся для задачи (от меньшего веса к большему).
    std::vector<std::vector<size_t>> subtasks(rocks.size() + 1);
    for (size_t i = 0; i <= rocks.size(); ++i) {
        subtasks[i].resize(max_weight + 1);
    }
    for (size_t i = 0; i <= rocks.size(); ++i) {
        subtasks[i][0] = 0;
    }
    for (size_t i = 0; i <= max_weight; ++i) {
        subtasks[0][i] = 0;
    }

    // Заполнение массива.
    for (size_t i = 1; i <= rocks.size(); ++i) {
        for (size_t j = 1; j <= max_weight; ++j) {
            if (j < rocks[i - 1]) {
                subtasks[i][j] = subtasks[i - 1][j];
            } else {
                subtasks[i][j] =
                    std::max(subtasks[i - 1][j], subtasks[i - 1][j - rocks[i - 1]] + rocks[i - 1]);
            }
        }
    }
    std::cout << subtasks[rocks.size()][max_weight] << "\n";

    // Обратный путь.
    int rock_i = rocks.size();
    int weight_i = max_weight;
    std::vector<int> result_rocks;
    while (subtasks[rock_i][weight_i] != 0) {
        while (subtasks[rock_i][weight_i] == subtasks[rock_i - 1][weight_i]) {
            --rock_i;
        }
        --rock_i;
        result_rocks.push_back(rocks[rock_i]);
        weight_i -= rocks[rock_i];
    }
    std::reverse(result_rocks.begin(), result_rocks.end());
    std::cout << result_rocks.size() << "\n";
    for (size_t i = 0; i < result_rocks.size(); ++i) {
        std::cout << result_rocks[i] << " ";
    }
}

int main() {
    size_t number_of_rocks, max_weight;
    std::vector<size_t> rocks;
    readData(&number_of_rocks, &max_weight, &rocks);
    printRocksSequenceUnderMaxWeight(max_weight, rocks);
    return 0;
}
