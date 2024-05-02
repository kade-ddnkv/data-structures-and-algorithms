#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <unordered_set>

int number_of_cities;
std::vector<int> used;
std::vector<std::vector<int>> graph;
std::vector<std::vector<int>> graph_t;

void readDataAndFillGraph() {
    int number_of_flights;
    std::cin >> number_of_cities >> number_of_flights;
    graph.resize(number_of_cities);
    graph_t.resize(number_of_cities);
    std::vector<std::pair<int, int>> flights(number_of_flights);
    for (int i = 0; i < number_of_flights; ++i) {
        int from, to;
        std::cin >> from >> to;
        flights[i] = {from - 1, to - 1};
    }
    std::sort(flights.begin(), flights.end());
    flights.erase(std::unique(flights.begin(), flights.end()), flights.end());
    for (size_t i = 0; i < flights.size(); ++i) {
        graph[flights[i].first].push_back(flights[i].second);
        graph_t[flights[i].second].push_back(flights[i].first);
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

std::vector<bool> comp_used;
std::vector<std::unordered_set<int>> comp_graph;
std::vector<std::unordered_set<int>> comp_access;

void deleteShortRoutes(int component) {
    std::unordered_set<int> extra_children;
    comp_used[component] = true;
    for (const int &child : comp_graph[component]) {
        comp_access[component].insert(child);
        if (!comp_used[child]) {
            deleteShortRoutes(child);
        }
        for (const int &node : comp_access[child]) {
            if (comp_graph[component].find(node) != comp_graph[component].end()) {
                extra_children.insert(node);
            } else {
                comp_access[component].insert(node);
            }
        }
    }
    for (const int &child : extra_children) {
        comp_graph[component].erase(child);
    }
}

void printNewFlights() {
    std::vector<std::pair<int, int>> result_flights;
    for (const std::vector<int> &component : components) {
        if (component.size() > 1) {
            for (size_t i = 0; i < component.size() - 1; ++i) {
                result_flights.push_back({component[i], component[i + 1]});
            }
            result_flights.push_back({component.back(), component[0]});
        }
    }
    for (size_t i = 0; i < comp_graph.size(); ++i) {
        for (const int &child_comp : comp_graph[i]) {
            result_flights.push_back({components[i][0], components[child_comp][0]});
        }
    }
    std::cout << result_flights.size() << "\n";
    for (auto flight : result_flights) {
        std::cout << flight.first + 1 << " " << flight.second + 1 << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Построение графа.
    readDataAndFillGraph();

    // Запуск первого прохода по графу.
    used.resize(number_of_cities, -1);
    for (int i = 0; i < number_of_cities; ++i) {
        if (used[i] == -1) {
            used[i] = 1;
            dfsStraight(i);
        }
    }

    // Запуск прохода по транспонированному графу.
    std::fill(used.begin(), used.end(), -1);
    int curr_component = 0;
    for (int i = 0; i < number_of_cities; ++i) {
        int curr_city = order[number_of_cities - 1 - i];
        if (used[curr_city] == -1) {
            components.push_back(std::vector<int>());
            used[curr_city] = curr_component;
            dfsReverse(curr_city, curr_component);
            ++curr_component;
        }
    }

    // Делаю граф из компонент.
    comp_graph.resize(curr_component);
    comp_used.resize(curr_component, false);
    comp_access.resize(curr_component);
    for (size_t i = 0; i < graph.size(); ++i) {
        for (int to : graph[i]) {
            if (used[i] != used[to]) {
                comp_graph[used[i]].insert(used[to]);
            }
        }
    }

    for (int i = 0; i < curr_component; ++i) {
        if (!comp_used[i]) {
            deleteShortRoutes(i);
        }
    }

    printNewFlights();
    return 0;
}
