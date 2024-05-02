#include <iostream>
#include <vector>

struct Arc {
    int from;
    int to;
    int weight;

    Arc(int from, int to, int weight) {
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};

class BellmanFord {
private:
    const int64_t k_pos_inf_ = INT64_MAX;
    const int64_t k_neg_inf_ = INT64_MIN;
    int number_of_vertices_, number_of_arcs_;
    std::vector<Arc> arcs_;
    std::vector<int64_t> distance_;
    int start_vertex_;

    // Метод вызывается после основного прохода алгоритма Беллмана-Форда.
    // Определяет вершины с дистанцией -inf.
    void revealNegInfVertices() {
        int iteration_index = 0;
        bool any_update;
        // Цикл максимум на n-1 итерацию.
        for (iteration_index = 0;; ++iteration_index) {
            any_update = false;
            for (int i = 0; i < number_of_arcs_; ++i) {
                if (distance_[arcs_[i].from] != k_pos_inf_ &&
                    distance_[arcs_[i].to] != k_neg_inf_) {
                    if (distance_[arcs_[i].from] == k_neg_inf_ ||
                        distance_[arcs_[i].from] + arcs_[i].weight < distance_[arcs_[i].to]) {
                        distance_[arcs_[i].to] = k_neg_inf_;
                        any_update = true;
                    }
                }
            }
            if (!any_update) {
                break;
            }
        }
    }

public:
    void readGraph() {
        std::cin >> number_of_vertices_ >> number_of_arcs_;
        arcs_.reserve(number_of_arcs_);
        for (int i = 0; i < number_of_arcs_; ++i) {
            int from, to, weight;
            std::cin >> from >> to >> weight;
            arcs_.push_back(Arc(from, to, weight));
        }
    }

    int calculateDistances(int start_vertex = 0) {
        start_vertex_ = start_vertex;
        distance_.resize(number_of_vertices_, k_pos_inf_);
        distance_[start_vertex_] = 0;
        int iteration_index;
        bool any_update;
        for (iteration_index = 0; iteration_index < number_of_vertices_; ++iteration_index) {
            any_update = false;
            for (int i = 0; i < number_of_arcs_; ++i) {
                if (distance_[arcs_[i].from] != k_pos_inf_) {
                    int64_t new_distance = distance_[arcs_[i].from] + arcs_[i].weight;
                    if (new_distance < distance_[arcs_[i].to]) {
                        distance_[arcs_[i].to] = new_distance;
                        any_update = true;
                    }
                }
            }
            if (!any_update) {
                break;
            }
        }
        // Если на n-ой итерации все еще было обновление, значит есть отрицательный цикл.
        if (any_update) {
            revealNegInfVertices();
        }
        return 0;
    }

    void printDistancesFromStartVertex() {
        for (int i = 1; i < number_of_vertices_; ++i) {
            if (distance_[i] == k_pos_inf_) {
                std::cout << "inf\n";
            } else if (distance_[i] == k_neg_inf_) {
                std::cout << "-inf\n";
            } else {
                std::cout << distance_[i] << "\n";
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    BellmanFord algo;
    algo.readGraph();
    algo.calculateDistances();
    algo.printDistancesFromStartVertex();
    return 0;
}
