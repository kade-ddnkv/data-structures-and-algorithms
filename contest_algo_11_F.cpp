#include <iostream>
#include <vector>
#include <algorithm>

void readData(int *board_length, int *number_of_cuts, std::vector<int> *cuts) {
    std::cin >> (*board_length) >> (*number_of_cuts);
    int cut;
    for (int i = 0; i < (*number_of_cuts); ++i) {
        std::cin >> cut;
        (*cuts).push_back(cut);
    }
}

// Map: ("индекс левого разреза", "индекс правого разреза") -> минимальная стоимость распила.
std::vector<std::vector<int>> map_range_cost;

// Рекурсивная функция с ленивой динамикой.
int cutRecursive(int left, int right, const std::vector<int> &cuts) {
    if (map_range_cost[left][right] == -1) {
        int min_cost = INT32_MAX;
        int cut_i = left + 1;
        while (cut_i < right) {
            // Намечен разрез.
            // Получить рекурсивно минимальную стоимость слева и справа от разреза.
            int cost = cuts[right] - cuts[left];
            cost += cutRecursive(left, cut_i, cuts) + cutRecursive(cut_i, right, cuts);
            if (cost < min_cost) {
                min_cost = cost;
            }
            ++cut_i;
        }
        if (min_cost == INT32_MAX) {
            min_cost = 0;
        }
        map_range_cost[left][right] = min_cost;
        return min_cost;
    } else {
        return map_range_cost[left][right];
    }
}

int getMinCostOfCuts(const std::vector<int> &cuts) {
    return cutRecursive(0, cuts.size() - 1, cuts);
}

int main() {
    int board_length, number_of_cuts;
    std::vector<int> cuts;
    // Добавляю разрезы слева от бревна и справа.
    cuts.push_back(0);
    readData(&board_length, &number_of_cuts, &cuts);
    cuts.push_back(board_length);

    // Инициализирую map для сохранения уже посчитанных значений.
    map_range_cost.resize(cuts.size());
    for (size_t i = 0; i < cuts.size(); ++i) {
        map_range_cost[i].resize(cuts.size(), -1);
    }

    std::cout << getMinCostOfCuts(cuts);
    return 0;
}
