#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>

int number_of_storages;
int number_of_maps;
std::vector<std::vector<std::vector<int>>> maps;

void readDataAndFillGraph() {
    std::cin >> number_of_storages >> number_of_maps;
    maps.resize(number_of_maps);
    for (int i = 0; i < number_of_maps; ++i) {
        maps[i].resize(number_of_storages);
    }

    int number_of_roads;
    int left, right;
    for (int i = 0; i < number_of_maps; ++i) {
        std::cin >> number_of_roads;
        for (int j = 0; j < number_of_roads; ++j) {
            std::cin >> left >> right;
            maps[i][left - 1].push_back(right - 1);
            maps[i][right - 1].push_back(left - 1);
        }
    }
}

struct House {
    int index;
    int level;

    House(int index, int level) {
        this->index = index;
        this->level = level;
    }
};

int routeMinCost() {
    std::deque<House> deque;
    std::vector<std::vector<int>> used;
    used.resize(number_of_maps, std::vector<int>(number_of_storages));

    deque.push_front(House(0, -1));
    deque.push_back(House(-5, 0));
    int sum_cost = 0;
    while (deque.size() != 1) {
        House curr = deque.front();
        if (curr.index == number_of_storages - 1) {
            return sum_cost;
        }
        if (curr.index == -5) {
            ++sum_cost;
            deque.pop_front();
            deque.push_back(House(-5, 0));
            continue;
        }

        deque.pop_front();
        if (curr.level != -1) {
            for (int connected : maps[curr.level][curr.index]) {
                if (used[curr.level][connected] == 0) {
                    deque.push_front(House(connected, curr.level));
                    used[curr.level][connected] = 1;
                }
            }
        }
        for (size_t i = 0; i < maps.size(); ++i) {
            if (!maps[i][curr.index].empty()) {
                for (int connected : maps[i][curr.index]) {
                    if (used[i][connected] == 0) {
                        deque.push_back(House(connected, i));
                        used[i][connected] = 1;
                    }
                }
                used[i][curr.index] = 1;
            }
        }
    }
    return -1;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    readData();
    std::cout << routeMinCost();
    return 0;
}
