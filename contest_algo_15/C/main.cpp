#include <iostream>
#include <vector>
#include <algorithm>

class FloydWarshall {
private:
    int number_of_vertices_, number_of_arcs_;
    std::vector<std::vector<int64_t>> distance_;

public:
    void readGraph() {
        std::cin >> number_of_vertices_ >> number_of_arcs_;
        distance_.resize(number_of_vertices_, std::vector<int64_t>(number_of_vertices_, -1));
        for (int i = 0; i < number_of_arcs_; ++i) {
            int from, to;
            std::cin >> from >> to;
            std::cin >> distance_[from][to];
        }
    }

    void calculateDistances() {
        for (int middle = 0; middle < number_of_vertices_; ++middle) {
            for (int start = 0; start < number_of_vertices_; ++start) {
                for (int end = 0; end < number_of_vertices_; ++end) {
                    if (distance_[start][middle] != -1 && distance_[middle][end] != -1) {
                        if (distance_[start][end] == -1) {
                            distance_[start][end] =
                                distance_[start][middle] + distance_[middle][end];
                        } else {
                            distance_[start][end] =
                                std::min(distance_[start][end],
                                         distance_[start][middle] + distance_[middle][end]);
                        }
                    }
                }
            }
        }
    }

    void printDistances() {
        for (int start = 0; start < number_of_vertices_; ++start) {
            for (int end = 0; end < number_of_vertices_; ++end) {
                if (start != end) {
                    std::cout << start << " " << end << " " << distance_[start][end] << "\n";
                }
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    FloydWarshall algo;
    algo.readGraph();
    algo.calculateDistances();
    algo.printDistances();
    return 0;
}
