#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

// Решение:
// Дуга в графе - это значит, если переставить время на первом сервере, время придется менять и
// на втором сервере тоже.
// Среди компонент сильной связности найти самый маленький сток.

int number_of_servers;
int number_of_hours;
std::vector<int> servers;
// Граф.
std::vector<std::vector<int>> graph;
// Транспонированный граф.
std::vector<std::vector<int>> graph_t;
// Использованность вершин при dfs и заодно запись номеров их компонент.
std::vector<int> used;

int plusOneHour(int curr_hour) {
    return (curr_hour + 1) % number_of_hours;
}

void readDataAndFillGraph() {
    int number_of_clients;
    std::cin >> number_of_servers >> number_of_clients >> number_of_hours;
    servers.resize(number_of_servers);
    graph.resize(number_of_servers);
    graph_t.resize(number_of_servers);
    for (int i = 0; i < number_of_servers; ++i) {
        std::cin >> servers[i];
    }
    std::vector<std::pair<int, int>> clients(number_of_clients);
    for (int i = 0; i < number_of_clients; ++i) {
        int left, right;
        std::cin >> left >> right;
        if (left > right) {
            std::swap(left, right);
        }
        clients[i] = {left - 1, right - 1};
    }
    std::sort(clients.begin(), clients.end());
    clients.erase(std::unique(clients.begin(), clients.end()), clients.end());
    for (size_t i = 0; i < clients.size(); ++i) {
        if (servers[clients[i].first] == plusOneHour(servers[clients[i].second])) {
            graph[clients[i].second].push_back(clients[i].first);
            graph_t[clients[i].first].push_back(clients[i].second);
        }
        if (servers[clients[i].second] == plusOneHour(servers[clients[i].first])) {
            graph[clients[i].first].push_back(clients[i].second);
            graph_t[clients[i].second].push_back(clients[i].first);
        }
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

std::vector<int> components_sizes;
std::vector<bool> component_is_stock;

// Среди компонент сильной связности, из которых ничего не исходит
// найти самую маленькую компоненту по размеру.
void findMinStockComponent() {
    for (size_t i = 0; i < graph.size(); ++i) {
        ++components_sizes[used[i]];
        for (int to : graph[i]) {
            if (used[i] != used[to]) {
                component_is_stock[used[i]] = false;
                break;
            }
        }
    }
    int min_stock_component = -1;
    int min_size = INT32_MAX;
    for (size_t i = 0; i < components_sizes.size(); ++i) {
        if (component_is_stock[i] && components_sizes[i] < min_size) {
            min_size = components_sizes[i];
            min_stock_component = i;
        }
    }
    std::vector<int> result_servers;
    for (int i = 0; i < number_of_servers; ++i) {
        if (used[i] == min_stock_component) {
            result_servers.push_back(i);
        }
    }
    std::sort(result_servers.begin(), result_servers.end());
    std::cout << result_servers.size() << "\n";
    for (const int server : result_servers) {
        std::cout << server + 1 << " ";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Построение графа.
    readDataAndFillGraph();

    // Запуск первого прохода по графу.
    used.resize(number_of_servers, -1);
    for (int i = 0; i < number_of_servers; ++i) {
        if (used[i] == -1) {
            used[i] = 1;
            dfsStraight(i);
        }
    }

    // Запуск прохода по транспонированному графу.
    std::fill(used.begin(), used.end(), -1);
    int curr_component = 0;
    for (int i = 0; i < number_of_servers; ++i) {
        int curr_server = order[number_of_servers - 1 - i];
        if (used[curr_server] == -1) {
            used[curr_server] = curr_component;
            dfsReverse(curr_server, curr_component);
            ++curr_component;
        }
    }
    component_is_stock.resize(curr_component, true);
    components_sizes.resize(curr_component);
    findMinStockComponent();
    return 0;
}
