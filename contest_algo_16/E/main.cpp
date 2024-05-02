#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <string>

// Сложность обработки одного запроса: O(V * E).

struct Arc {
    int from;
    int to;
    int cap;
    int flow;
};

class Dinic {
private:
    int number_of_vertices_, number_of_queries_;
    std::vector<int> distance_;
    std::vector<int> next_edge_;
    std::vector<Arc> arcs_;
    std::vector<std::vector<int>> graph_;

    const int k_inf_ = INT32_MAX;

    int source_, target_;

    std::unordered_map<std::string, int> names_map_;

    bool bfs() {
        std::fill(distance_.begin(), distance_.end(), -1);
        std::queue<int> queue;
        queue.push(source_);
        distance_[source_] = 0;
        while (!queue.empty()) {
            int curr = queue.front();
            queue.pop();
            if (curr == target_) {
                continue;
            }
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

    int readName() {
        std::string name;
        std::cin >> name;
        if (names_map_.find(name) == names_map_.end()) {
            names_map_[name] = names_map_.size();
        }
        return names_map_[name];
    }

    void handleSourceEqualsTarget() {
        std::cout << 0 << "\n";
    }

public:
    void readData() {
        int number_of_arcs;
        std::cin >> number_of_vertices_ >> number_of_arcs >> number_of_queries_;

        distance_.resize(number_of_vertices_);
        next_edge_.resize(number_of_vertices_);
        graph_.resize(number_of_vertices_);
        for (int i = 0; i < number_of_arcs; ++i) {
            int vertex1 = readName();
            int vertex2 = readName();
            addArc(vertex1, vertex2, 1);
            addArc(vertex2, vertex1, 1);
        }
    }

    int findMaxFlow() {
        for (Arc &arc : arcs_) {
            arc.flow = 0;
        }
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

    void handleQueries() {
        for (int i = 0; i < number_of_queries_; ++i) {
            source_ = readName();
            target_ = readName();
            if (source_ == target_) {
                handleSourceEqualsTarget();
            } else {
                std::cout << findMaxFlow() << "\n";
            }
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Dinic algo;
    algo.readData();
    algo.handleQueries();
    return 0;
}
