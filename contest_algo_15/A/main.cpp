#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <functional>

class Dijkstra {
private:
    int number_of_vertices_, number_of_arcs_;
    std::vector<std::vector<std::pair<int, int>>> graph_;
    std::priority_queue<std::pair<int64_t, int>, std::vector<std::pair<int64_t, int>>,
                        std::greater<std::pair<int64_t, int>>>
        queue_;
    std::vector<int64_t> distance_;

public:
    void readGraph() {
        std::cin >> number_of_vertices_ >> number_of_arcs_;
        graph_.resize(number_of_vertices_);
        for (int i = 0; i < number_of_arcs_; ++i) {
            int from, to, weight;
            std::cin >> from >> to >> weight;
            graph_[from].push_back({to, weight});
        }
    }

    void calculateDistances() {
        distance_.resize(number_of_vertices_, -1);
        distance_[0] = 0;
        queue_.push({0, 0});
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
                    queue_.push({new_distance, child.first});
                }
            }
        }
    }

    void printDistances() {
        for (int i = 1; i < number_of_vertices_; ++i) {
            std::cout << distance_[i] << "\n";
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Dijkstra algo;
    algo.readGraph();
    algo.calculateDistances();
    algo.printDistances();
    return 0;
}
