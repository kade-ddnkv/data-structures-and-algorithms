#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <functional>
#include <string>

int sumOfDigits(int number) {
    int sum = 0;
    while (number != 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

std::vector<std::vector<std::pair<int, int>>> createGraphOfNumbers(int max_numb) {
    std::vector<std::vector<std::pair<int, int>>> graph(max_numb + 1);

    for (int curr_numb = 1; curr_numb <= max_numb; ++curr_numb) {
        std::string curr_numb_str = std::to_string(curr_numb);
        for (size_t len = 1; len <= curr_numb_str.size(); ++len) {
            for (size_t start = 0; start <= curr_numb_str.size() - len; ++start) {
                std::string substr = curr_numb_str.substr(start, len);
                int subnumb = std::stoi(substr);
                if (subnumb > 0) {
                    if (curr_numb + subnumb <= max_numb) {
                        graph[curr_numb].push_back({curr_numb + subnumb, sumOfDigits(subnumb)});
                    }
                    if (curr_numb - subnumb > 0) {
                        graph[curr_numb].push_back({curr_numb - subnumb, sumOfDigits(subnumb)});
                    }
                }
            }
        }
    }

    return graph;
}

class Dijkstra {
private:
    int number_of_vertices_;
    int start_vertex_;
    std::vector<std::vector<std::pair<int, int>>> graph_;
    std::priority_queue<std::pair<int64_t, int>, std::vector<std::pair<int64_t, int>>,
                        std::greater<std::pair<int64_t, int>>>
        queue_;
    std::vector<int64_t> distance_;
    std::vector<int> backtrace_;

public:
    void setGraph(const std::vector<std::vector<std::pair<int, int>>> &graph, int start_vertex) {
        number_of_vertices_ = graph.size();
        graph_ = graph;
        start_vertex_ = start_vertex;
    }

    void calculateDistances() {
        backtrace_.resize(number_of_vertices_, -1);
        distance_.resize(number_of_vertices_, -1);
        distance_[start_vertex_] = 0;
        queue_.push({0, start_vertex_});
        while (!queue_.empty()) {
            int64_t weight = queue_.top().first;
            int index_of_min = queue_.top().second;
            queue_.pop();
            if (weight > distance_[index_of_min]) {
                continue;
            }

            for (const std::pair<int, int> &child : graph_[index_of_min]) {
                int64_t new_distance = distance_[index_of_min] + child.second;
                if (new_distance < distance_[child.first] || distance_[child.first] == -1) {
                    distance_[child.first] = new_distance;
                    backtrace_[child.first] = index_of_min;
                    queue_.push({new_distance, child.first});
                }
            }
        }
    }

    void printMinPathToVertex(int vertex_end) {
        std::cout << distance_[vertex_end] << "\n";

        std::vector<int> deltas;
        int curr = vertex_end;
        while (backtrace_[curr] != -1) {
            deltas.push_back(curr - backtrace_[curr]);
            curr = backtrace_[curr];
        }
        std::cout << deltas.size() << "\n";
        for (int i = deltas.size() - 1; i >= 0; --i) {
            std::cout << (deltas[i] > 0 ? "+" : "") << deltas[i] << "\n";
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int max_numb, begin_numb, end_numb;
    std::cin >> max_numb >> begin_numb >> end_numb;

    if (begin_numb == end_numb) {
        throw std::exception();
    }

    auto graph = createGraphOfNumbers(max_numb);

    Dijkstra algo;
    algo.setGraph(graph, begin_numb);
    algo.calculateDistances();
    algo.printMinPathToVertex(end_numb);
    return 0;
}
