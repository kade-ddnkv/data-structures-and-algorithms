#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>

std::vector<int> readBarGraphFromConsole() {
    std::string input_line;
    int bars_number;
    std::cin >> bars_number;
    std::vector<int> bar_graph(bars_number);

    for (int i = 0; i < bars_number; ++i) {
        std::cin >> bar_graph[i];
    }
    return bar_graph;
}

struct Column {
    int index;
    int height;

    Column(int index, int height) : index(index), height(height) {
    }
};

int64_t getMaxRectangleSquare(std::vector<int> bar_graph) {
    int64_t max_square = 0;
    std::stack<Column> stack;
    stack.push(Column(0, -1));

    auto pop_last_column{[&max_square, &stack](int right_coord) {
        int64_t local_height = stack.top().height;
        stack.pop();
        int left_coord = stack.top().index + 1;
        max_square = std::max(max_square, (right_coord - left_coord + 1) * local_height);
    }};

    for (size_t i = 0; i < bar_graph.size(); ++i) {
        while (stack.top().height >= bar_graph[i]) {
            pop_last_column(i);
        }
        stack.push(Column(i + 1, bar_graph[i]));
    }
    while (stack.top().height >= 0) {
        pop_last_column(bar_graph.size());
    }
    return max_square;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<int> bar_graph = readBarGraphFromConsole();
    std::cout << getMaxRectangleSquare(bar_graph);

    return 0;
}
