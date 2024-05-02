#include <iostream>
#include <string>
#include <map>
#include <vector>

class DisjointSet {
private:
    std::vector<int> parents_;

public:
    explicit DisjointSet(int number_of_vertices) {
        parents_.resize(number_of_vertices);
        for (size_t i = 0; i < parents_.size(); ++i) {
            parents_[i] = i;
        }
    }

    void unite(int child_index, int parent_index) {
        parents_[child_index] = parent_index;
    }

    int getDepth(int index) {
        int depth = 0;
        while (parents_[index] != index) {
            ++depth;
            index = parents_[index];
        }
        return depth;
    }
};

int main() {
    int number_of_nodes;
    std::cin >> number_of_nodes;
    std::map<std::string, int> dict;
    DisjointSet disjoint_set(number_of_nodes);

    // Ввод и сразу добавление вершин в СНМ.
    for (int i = 0; i < number_of_nodes - 1; ++i) {
        std::string child, parent;
        std::cin >> child >> parent;
        if (dict.find(parent) == dict.end()) {
            dict[parent] = dict.size();
        }
        if (dict.find(child) == dict.end()) {
            dict[child] = dict.size();
        }
        disjoint_set.unite(dict[child], dict[parent]);
    }

    // Вывод.
    for (auto pair : dict) {
        std::cout << pair.first << " " << disjoint_set.getDepth(pair.second) << "\n";
    }
    return 0;
}
