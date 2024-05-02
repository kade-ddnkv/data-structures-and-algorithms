#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>

// Идея: каждая вершина разделяется на две.
// В первой вершине будут все входящие дуги.
// Во второй вершине будут все исходящие дуги.
// Между подвершинами проводится дуга.

struct Point {
    int x;
    int y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {

template <>
struct hash<Point> {
    std::size_t operator()(const Point &point) const {
        size_t h = (size_t(point.x) << 32) + size_t(point.y);
        h *= 1231231557ull;
        h ^= (h >> 32);
        return h;
    }
};

}  // namespace std

struct PointExpanded {
    int in_i;
    int out_i;
};

struct Arc {
    int from;
    int to;
    int cap;
    int flow;
};

class Dinic {
private:
    int number_of_vertices_ = 0;
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

    PointExpanded readPoint(std::unordered_map<Point, PointExpanded> *mapper) {
        Point point;
        std::cin >> point.x >> point.y;
        if (mapper->find(point) == mapper->end()) {
            (*mapper)[point] = {number_of_vertices_, number_of_vertices_ + 1};
            number_of_vertices_ += 2;
        }
        return (*mapper)[point];
    }

    void addArc(int from, int to, int cap) {
        Arc a1 = {from, to, cap, 0};
        Arc a2 = {to, from, 0, 0};
        graph_[from].push_back(arcs_.size());
        arcs_.push_back(a1);
        graph_[to].push_back(arcs_.size());
        arcs_.push_back(a2);
    }

public:
    void readData() {
        int number_of_edges;
        std::cin >> number_of_edges;
        std::vector<std::pair<int, int>> arcs;
        std::unordered_map<Point, PointExpanded> point_mapper;
        for (int i = 0; i < number_of_edges; ++i) {
            PointExpanded start = readPoint(&point_mapper);
            PointExpanded end = readPoint(&point_mapper);
            arcs.push_back({start.out_i, end.in_i});
            arcs.push_back({end.out_i, start.in_i});
        }
        source_ = readPoint(&point_mapper).in_i;
        target_ = readPoint(&point_mapper).out_i;

        distance_.resize(number_of_vertices_);
        next_edge_.resize(number_of_vertices_);
        graph_.resize(number_of_vertices_);
        for (auto pair : point_mapper) {
            int cap = 1;
            if (pair.second.in_i == source_ || pair.second.out_i == target_) {
                cap = k_inf_;
            }
            addArc(pair.second.in_i, pair.second.out_i, cap);
        }
        for (size_t i = 0; i < arcs.size(); ++i) {
            addArc(arcs[i].first, arcs[i].second, 1);
        }
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

    Dinic algo;
    algo.readData();
    std::cout << algo.findMaxFlow();
    return 0;
}
