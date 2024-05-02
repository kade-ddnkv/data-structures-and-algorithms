#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

int number_of_persons;
// Вектор для определения использованности при dfs.
std::vector<int> used;
// Map копий актера с их именами.
std::vector<std::string> names;
// Граф.
std::vector<std::vector<int>> graph;
// Транспонированный граф.
std::vector<std::vector<int>> graph_t;

void readDataAndFillGraph() {
    int number_of_connections;
    std::cin >> number_of_persons >> number_of_connections;
    names.resize(number_of_persons);
    graph.resize(number_of_persons);
    graph_t.resize(number_of_persons);
    for (int i = 0; i < number_of_persons; ++i) {
        int person_number;
        std::string name;
        std::cin >> person_number;
        std::getline(std::cin, name);
        names[person_number] = name.substr(1);
    }
    for (int i = 0; i < number_of_connections; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph[from].push_back(to);
        graph_t[to].push_back(from);
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

std::vector<std::vector<int>> components;

void dfsReverse(int vertex, int component) {
    components.back().push_back(vertex);
    for (int to : graph_t[vertex]) {
        if (used[to] == -1) {
            used[to] = component;
            dfsReverse(to, component);
        }
    }
}

void printComponents() {
    // Сортировка.
    auto less_by_first_item = [](const std::vector<int> &first, const std::vector<int> &second) {
        return names[first[0]].compare(names[second[0]]) < 0;
    };
    auto less_by_names = [](const int &first, const int &second) {
        return names[first].compare(names[second]) < 0;
    };
    for (size_t i = 0; i < components.size(); ++i) {
        std::sort(components[i].begin(), components[i].end(), less_by_names);
    }
    std::sort(components.begin(), components.end(), less_by_first_item);

    // Вывод.
    for (const std::vector<int> &component : components) {
        for (const int &vertex : component) {
            std::cout << names[vertex] << "\n";
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Построение графа.
    readDataAndFillGraph();

    // Запуск первого прохода по графу.
    used.resize(number_of_persons, -1);
    for (int i = 0; i < number_of_persons; ++i) {
        if (used[i] == -1) {
            used[i] = 1;
            dfsStraight(i);
        }
    }

    // Запуск прохода по транспонированному графу.
    std::fill(used.begin(), used.end(), -1);
    int curr_component = 0;
    for (int i = 0; i < number_of_persons; ++i) {
        int curr_city = order[number_of_persons - 1 - i];
        if (used[curr_city] == -1) {
            components.push_back(std::vector<int>());
            used[curr_city] = curr_component;
            dfsReverse(curr_city, curr_component);
            ++curr_component;
        }
    }

    printComponents();
    return 0;
}
