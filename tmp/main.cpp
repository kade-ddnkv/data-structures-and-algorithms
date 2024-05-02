#include <iostream>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::queue;
using std::unordered_map;
using std::vector;

const int kInfinity = 1e9 + 1;

struct HashCalculator {
    inline size_t operator()(const pair<int, int> &pair) const {
        auto first_hash = std::hash<int>{}(pair.first);
        auto second_hash = std::hash<int>{}(pair.second);
        return (first_hash == second_hash ? first_hash : (first_hash << 32) ^ second_hash);
    }
};

class Network {
private:
    vector<vector<pair<int, bool>>> graph_;
    vector<int> distances_, children_;
    int source_, stock_;
    unordered_map<pair<int, int>, int, HashCalculator> points_;

    bool isInfinityEdge(const int node, const int child) {
        return (node == source_ - 1 && child == source_) ||
               (node == source_ && child == source_ - 1) ||
               (node == stock_ + 1 && child == stock_) || (node == stock_ && child == stock_ + 1);
    }

    bool searchForBreadthFirst() {
        distances_.assign(graph_.size(), kInfinity);
        distances_[source_] = 0;
        queue<int> queue;
        queue.push(source_);
        while (!queue.empty() && distances_[stock_] == kInfinity) {
            int node = queue.front();
            queue.pop();
            for (const auto &child : graph_[node]) {
                if (distances_[child.first] == kInfinity && child.second) {
                    distances_[child.first] = distances_[node] + 1;
                    queue.push(child.first);
                }
            }
        }
        return distances_[stock_] != kInfinity;
    }

    bool searchForDepthFirst(const int node) {
        while (children_[node] < graph_[node].size()) {
            if (distances_[graph_[node][children_[node]].first] == distances_[node] + 1 &&
                graph_[node][children_[node]].second) {
                if (graph_[node][children_[node]].first == stock_ ||
                    searchForDepthFirst(graph_[node][children_[node]].first)) {
                    graph_[node][children_[node]].second =
                        isInfinityEdge(node, graph_[node][children_[node]].first);
                    return true;
                }
            }
            ++children_[node];
        }
        return false;
    }

public:
    void readData() {
        int edges;
        cin >> edges;
        if (!edges) {
            return;
        }
        graph_.resize(edges * 4);
        int last_node = -1, first_out, second_in;
        int first_in, second_out;
        pair<int, int> first_point, second_point;
        for (int index = 0; index < edges; ++index) {
            cin >> first_point.first >> first_point.second >> second_point.first >>
                second_point.second;
            if (!points_.count(first_point)) {
                points_[first_point] = ++last_node;
                graph_[last_node].push_back({last_node + 1, true});
                first_in = last_node;
                first_out = ++last_node;
            } else {
                first_in = points_[first_point];
                first_out = points_[first_point] + 1;
            }
            if (!points_.count(second_point)) {
                second_in = points_[second_point] = ++last_node;
                graph_[last_node].push_back({last_node + 1, true});
                ++last_node;
                second_out = last_node + 1;
            } else {
                second_in = points_[second_point];
                second_out = points_[second_point] + 1;
            }
            if (first_out == second_in + 1) {
                continue;
            }
            graph_[first_out].push_back({second_in, true});
            graph_[second_out].push_back({first_in, true});
        }
        graph_.resize(last_node + 1);
    }

    // By Dinic with scaling.
    void printMaxFlow() {
        if (graph_.empty()) {
            cout << 0;
            return;
        }
        pair<int, int> first_point, second_point;
        cin >> first_point.first >> first_point.second >> second_point.first >> second_point.second;
        if (!points_.count(first_point) || !points_.count(second_point)) {
            cout << 0;
            return;
        }
        source_ = points_[first_point] + 1;
        stock_ = points_[second_point];
        int max_flow = 0;
        while (searchForBreadthFirst()) {
            children_.assign(graph_.size(), 0);
            while (searchForDepthFirst(source_)) {
                ++max_flow;
            }
        }
        cout << max_flow;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    Network network;
    network.readData();
    network.printMaxFlow();
    return 0;
}
