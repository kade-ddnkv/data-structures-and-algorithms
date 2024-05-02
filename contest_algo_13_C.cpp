#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

std::stack<int> stack;
std::vector<int> used;

std::vector<std::string> names;

std::vector<std::vector<int>> graph;

struct LexicographicalLess {
    bool operator()(const int& first, const int& second) {
        return (names[first].compare(names[second]) < 0);
    }
};

int readData() {
    int number_of_persons, number_of_connections;
    std::cin >> number_of_persons >> number_of_connections;
    graph.resize(number_of_persons, std::vector<int>());
    used.resize(number_of_persons);
    names.resize(number_of_persons);
    for (int i = 0; i < number_of_persons; ++i) {
        int person_number;
        std::string name, surname;
        std::cin >> person_number;
        std::getline(std::cin, name);
        names[person_number] = name.substr(1);
    }
    for (int i = 0; i < number_of_connections; ++i) {
        int left;
        int right;
        std::cin >> left >> right;
        graph[left].push_back(right);
        graph[right].push_back(left);
    }
    int start_index;
    std::cin >> start_index;
    return start_index;
}

void dfs(int index) {
    std::cout << names[index] << "\n";
    used[index] = 1;
    std::vector<int>& curr_connections = graph[index];
    for (size_t i = 0; i < curr_connections.size(); ++i) {
        if (used[curr_connections[i]] != 1) {
            dfs(curr_connections[i]);
        }
    }
}

void solve(int start_index) {
    for (size_t i = 0; i < graph.size(); ++i) {
        std::sort(graph[i].begin(), graph[i].end(), LexicographicalLess());
    }
    dfs(start_index);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int start_index = readData();
    solve(start_index);
    return 0;
}
