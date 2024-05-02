#include <iostream>
#include <vector>
#include <algorithm>

int number_of_vertices;
// Вектор для определения использованности при dfs и заодно определение причастности к компоненте.
std::vector<int> used;
// Граф.
std::vector<std::vector<int>> graph;
// Транспонированный граф.
std::vector<std::vector<int>> graph_t;

void readDataAndFillGraph() {
    int number_of_connections;
    std::cin >> number_of_vertices >> number_of_connections;
    graph.resize(number_of_vertices);
    graph_t.resize(number_of_vertices);
    for (int i = 0; i < number_of_connections; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph[from - 1].push_back(to - 1);
        graph_t[to - 1].push_back(from - 1);
    }
}

std::vector<int> order;

void dfsStraight(int vertex) {
    for (int to : graph[vertex]) {
        if (used[to] == -1) {
            used[to] = 1;
            dfsStraight(to);
        }
    }
    order.push_back(vertex);
}

void dfsReverse(int vertex, int component) {
    for (int to : graph_t[vertex]) {
        if (used[to] == -1) {
            used[to] = component;
            dfsReverse(to, component);
        }
    }
}

void printComponentsNumbers() {
    for (int i = 0; i < number_of_vertices; ++i) {
        std::cout << used[i] + 1 << " ";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Построение графа.
    readDataAndFillGraph();

    // Запуск первого прохода по графу.
    used.resize(number_of_vertices, -1);
    for (int i = 0; i < number_of_vertices; ++i) {
        if (used[i] == -1) {
            used[i] = 1;
            dfsStraight(i);
        }
    }

    // Запуск прохода по транспонированному графу.
    std::fill(used.begin(), used.end(), -1);
    int curr_component = 0;
    for (int i = 0; i < number_of_vertices; ++i) {
        int curr_city = order[number_of_vertices - 1 - i];
        if (used[curr_city] == -1) {
            used[curr_city] = curr_component;
            dfsReverse(curr_city, curr_component);
            ++curr_component;
        }
    }

    std::cout << curr_component << "\n";
    printComponentsNumbers();

    return 0;
}
