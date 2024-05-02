#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Решение:
// Найти все истоки, создать корень, к которому привязать все истоки,
// начать топологическую сортировку от корня.

int number_of_clothes;
// Вектор для определения использованности при dfs.
std::vector<int> used;
// Отображение номеров вершин на их имена.
std::vector<std::string> names;
// Граф.
std::vector<std::vector<int>> graph;

void readDataAndFillGraph() {
    int number_of_connections;
    std::cin >> number_of_clothes >> number_of_connections;
    graph.resize(number_of_clothes);
    names.resize(number_of_clothes);
    for (int i = 0; i < number_of_clothes; ++i) {
        int cloth_number;
        std::string name;
        std::cin >> cloth_number;
        std::getline(std::cin, name);
        names[cloth_number - 1] = name.substr(1);
    }
    for (int i = 0; i < number_of_connections; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph[from - 1].push_back(to - 1);
    }
}

void sortGraph() {
    auto less_by_names = [](const int &first, const int &second) {
        return names[first].compare(names[second]) < 0;
    };
    for (int i = 0; i < number_of_clothes; ++i) {
        std::sort(graph[i].begin(), graph[i].end(), less_by_names);
    }
}

std::vector<int> order;
std::vector<bool> is_origin;

void dfsRevealOrigins(int vertex) {
    for (int to : graph[vertex]) {
        is_origin[to] = false;
        if (used[to] == -1) {
            used[to] = 1;
            dfsRevealOrigins(to);
        }
    }
}

void dfsStraight(int vertex) {
    for (int to : graph[vertex]) {
        if (used[to] == -1) {
            used[to] = 1;
            dfsStraight(to);
        }
    }
    order.push_back(vertex);
}

void printTopologicalOrder() {
    // Выводить все, кроме последнего - это dummy-корень.
    for (int i = number_of_clothes - 2; i >= 0; --i) {
        std::cout << names[order[i]] << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Построение графа.
    readDataAndFillGraph();

    // Определение истоков и создание вершины-корня.
    used.resize(number_of_clothes, -1);
    is_origin.resize(number_of_clothes, true);
    for (int i = 0; i < number_of_clothes; ++i) {
        if (used[i] == -1) {
            used[i] = 1;
            dfsRevealOrigins(i);
        }
    }
    number_of_clothes += 1;
    graph.push_back(std::vector<int>());
    used.push_back(-1);
    for (size_t i = 0; i < is_origin.size(); ++i) {
        if (is_origin[i]) {
            graph.back().push_back(i);
        }
    }

    sortGraph();

    // Запуск прохода по графу топологической сортировки.
    std::fill(used.begin(), used.end(), -1);
    used[number_of_clothes - 1] = 1;
    dfsStraight(number_of_clothes - 1);

    printTopologicalOrder();

    return 0;
}
