#include <iostream>

class DisjointSet {
    int* parents_;
    int min_unites_number_;
    int unites_count_ = 0;

public:
    explicit DisjointSet(int number_of_elements) {
        parents_ = new int[number_of_elements];
        for (int i = 0; i < number_of_elements; ++i) {
            parents_[i] = i;
        }
        min_unites_number_ = number_of_elements - 1;
    }

    int find(int elem) {
        if (parents_[elem] == elem) {
            return elem;
        } else {
            return find(parents_[elem]);
        }
    }

    // Возвращает bool - является ли граф связным (= 1 компонента смежности).
    bool kruskalUnite(int left, int right) {
        int left_head = find(left);
        int right_head = find(right);

        if (left_head != right_head) {
            ++unites_count_;
            parents_[right_head] = left_head;
        }
        if (unites_count_ == min_unites_number_) {
            return true;
        }
        return false;
    }

    ~DisjointSet() {
        delete[] parents_;
    }
};

void handleConsoleCommands(DisjointSet* set, int islands_number) {
    int bridges_number;
    std::cin >> bridges_number;
    int left_island, right_island;

    bool fully_connected = islands_number == 1;
    int bridges = 0;
    for (; bridges < bridges_number && !fully_connected; ++bridges) {
        std::cin >> left_island >> right_island;
        fully_connected = set->kruskalUnite(left_island, right_island);
    }
    std::cout << bridges;
}

int main() {
    int islands_number;
    std::cin >> islands_number;
    DisjointSet set(islands_number);

    handleConsoleCommands(&set, islands_number);
    return 0;
}
