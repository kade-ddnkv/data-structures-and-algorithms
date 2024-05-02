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

    void createGraphFromChessBoard(const std::vector<std::vector<char>> &board, const int &height,
                                   const int &width, const int &white_price, const int &black_price,
                                   const int &grey_price) {
        number_of_vertices_ = height * width + 2;
        graph_.resize(number_of_vertices_);
        distance_.resize(number_of_vertices_);
        next_edge_.resize(number_of_vertices_);
        source_ = number_of_vertices_ - 2;
        target_ = number_of_vertices_ - 1;

        // Разбить элементы на двудольный граф (шахматная доска).
        // Добавить дуги из черных клеток в белые и обратно.
        for (int yy = 0; yy < height; ++yy) {
            for (int xx = 0; xx < width; ++xx) {
                if (board[yy][xx] == 'B') {
                    // Добавить дугу из фиктивного истока.
                    addArc(source_, yy * width + xx, white_price);
                } else if (board[yy][xx] == 'W') {
                    // Добавить дугу в фиктивный сток.
                    addArc(yy * width + xx, target_, black_price);
                }
                // Сверху.
                if (yy != 0) {
                    addArc(yy * width + xx, (yy - 1) * width + xx, grey_price);
                }
                // Снизу.
                if (yy != height - 1) {
                    addArc(yy * width + xx, (yy + 1) * width + xx, grey_price);
                }
                // Слева.
                if (xx != 0) {
                    addArc(yy * width + xx, yy * width + (xx - 1), grey_price);
                }
                // Справа.
                if (xx != width - 1) {
                    addArc(yy * width + xx, yy * width + (xx + 1), grey_price);
                }
            }
        }
    }

public:
    void readData() {
        int height, width;
        int white_price, black_price, grey_price;
        std::cin >> height >> width;
        std::cin >> white_price >> black_price >> grey_price;
        std::vector<std::vector<char>> board(height, std::vector<char>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cin >> board[y][x];
            }
        }
        createGraphFromChessBoard(board, height, width, white_price, black_price, grey_price);
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
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MultipleSourcesTargetsDinic algo;
    algo.readData();
    std::cout << algo.findMaxFlow();
    return 0;
}
