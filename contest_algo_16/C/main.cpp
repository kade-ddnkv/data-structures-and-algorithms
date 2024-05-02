#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

struct Arc {
    int from;
    int to;
    int cap;
    int flow;
};

class MultipleSourcesTargetsDinic {
private:
    int number_of_vertices_;
    int sum_of_source_valencies_ = 0;
    int sum_of_target_valencies_ = 0;
    std::vector<int> distance_;
    std::vector<int> next_edge_;
    std::vector<Arc> arcs_;
    std::vector<std::vector<int>> graph_;

    const int k_inf_ = INT32_MAX;

    int source_, target_;

    bool bfs() {
        std::fill(distance_.begin(), distance_.end(), -1);
        std::queue<int> queue;
        queue.push(source_);
        distance_[source_] = 0;
        while (!queue.empty()) {
            int curr = queue.front();
            queue.pop();
            for (const int &arc_i : graph_[curr]) {
                Arc &arc = arcs_[arc_i];
                int to = arc.to;
                if (distance_[to] == -1 && arc.flow < arc.cap) {
                    queue.push(to);
                    distance_[to] = distance_[curr] + 1;
                }
            }
        }
        return distance_[target_] != -1;
    }

    int dfs(int curr, int flow) {
        if (flow == 0) {
            return 0;
        }
        if (curr == target_) {
            return flow;
        }
        for (; next_edge_[curr] < static_cast<int>(graph_[curr].size()); ++next_edge_[curr]) {
            int &arc_i = graph_[curr][next_edge_[curr]];
            Arc &arc = arcs_[arc_i];
            int to = arc.to;
            if (distance_[to] != distance_[curr] + 1) {
                continue;
            }
            int flow_to_target = dfs(to, std::min(flow, arc.cap - arc.flow));
            if (flow_to_target != 0) {
                arc.flow += flow_to_target;
                arcs_[arc_i ^ 1].flow -= flow_to_target;
                return flow_to_target;
            }
        }
        return 0;
    }

    void addArc(int from, int to, int cap) {
        Arc a1 = {from, to, cap, 0};
        Arc a2 = {to, from, 0, 0};
        graph_[from].push_back(arcs_.size());
        arcs_.push_back(a1);
        graph_[to].push_back(arcs_.size());
        arcs_.push_back(a2);
    }

    int findMaxFlow() {
        int flow = 0;
        bool inf_loop = true;
        while (inf_loop) {
            if (!bfs()) {
                break;
            }
            std::fill(next_edge_.begin(), next_edge_.end(), 0);
            while (int flow_to_target = dfs(source_, k_inf_)) {
                flow += flow_to_target;
            }
        }
        return flow;
    }

    int getElementValency(char element) {
        switch (element) {
            case 'H':
                return 1;
            case 'O':
                return 2;
            case 'N':
                return 3;
            case 'C':
                return 4;
            default:
                throw std::exception();
        }
    }

    void createGraphFromPaper(const std::vector<std::vector<char>> &paper, const int &height,
                              const int &width) {
        number_of_vertices_ = height * width + 2;
        graph_.resize(number_of_vertices_);
        distance_.resize(number_of_vertices_);
        next_edge_.resize(number_of_vertices_);
        source_ = number_of_vertices_ - 2;
        target_ = number_of_vertices_ - 1;

        // Разбить элементы на двудольный граф (шахматная доска).
        // Добавить дуги из черных клеток в белые.
        for (int yy = 0; yy < height; ++yy) {
            for (int xx = 0; xx < width; ++xx) {
                if (paper[yy][xx] != '.') {
                    int element_valency = getElementValency(paper[yy][xx]);
                    // Черная клетка.
                    if ((yy + xx) % 2 == 0) {
                        sum_of_source_valencies_ += element_valency;
                        // Добавить дугу из фиктивного истока.
                        addArc(source_, yy * width + xx, element_valency);
                        // Сверху.
                        if (yy != 0 && paper[yy - 1][xx] != '.') {
                            addArc(yy * width + xx, (yy - 1) * width + xx, 1);
                        }
                        // Снизу.
                        if (yy != height - 1 && paper[yy + 1][xx] != '.') {
                            addArc(yy * width + xx, (yy + 1) * width + xx, 1);
                        }
                        // Слева.
                        if (xx != 0 && paper[yy][xx - 1] != '.') {
                            addArc(yy * width + xx, yy * width + (xx - 1), 1);
                        }
                        // Справа.
                        if (xx != width - 1 && paper[yy][xx + 1] != '.') {
                            addArc(yy * width + xx, yy * width + (xx + 1), 1);
                        }
                    } else {
                        sum_of_target_valencies_ += element_valency;
                        // Добавить дугу в фиктивный сток.
                        addArc(yy * width + xx, target_, element_valency);
                    }
                }
            }
        }
    }

public:
    void readData() {
        int height, width;
        std::cin >> height >> width;
        std::vector<std::vector<char>> paper(height, std::vector<char>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cin >> paper[y][x];
            }
        }
        createGraphFromPaper(paper, height, width);
    }

    bool isChemicalValid() {
        if (sum_of_source_valencies_ != sum_of_target_valencies_) {
            return false;
        }
        return findMaxFlow() == sum_of_target_valencies_;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MultipleSourcesTargetsDinic algo;
    algo.readData();
    std::cout << (algo.isChemicalValid() ? "Valid" : "Invalid");
    return 0;
}
