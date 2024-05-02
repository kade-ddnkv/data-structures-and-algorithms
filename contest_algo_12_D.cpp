#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> subtasks;

const int kMaxNumberOfTrees = 2000000 + 10;
const int kModule = 1000000007;

void computeSubtask(int tree_level) {
    subtasks[tree_level] =
        std::max((subtasks[tree_level - 1] + subtasks[tree_level - 2] * 2LL) % kModule,
                 (subtasks[tree_level - 4] * 4LL + subtasks[tree_level - 3] * 4LL +
                  subtasks[tree_level - 2] + 4) %
                     kModule);
}

void handleQueries() {
    subtasks.resize(kMaxNumberOfTrees, -1);
    subtasks[0] = 0;
    subtasks[1] = 0;
    subtasks[2] = 0;
    subtasks[3] = 4;

    // Ввод.
    int number_of_queries;
    std::cin >> number_of_queries;
    std::vector<int> queries(number_of_queries);
    for (int i = 0; i < number_of_queries; ++i) {
        std::cin >> queries[i];
    }
    // Подсчет всех подзадач вплоть до максимального уровня дерева.
    int max_tree_level = *std::max_element(queries.begin(), queries.end());
    for (int tree_level = 4; tree_level <= max_tree_level; ++tree_level) {
        computeSubtask(tree_level);
    }
    // Вывод.
    for (const int tree_level : queries) {
        std::cout << subtasks[tree_level] << "\n";
    }
}

int main() {
    handleQueries();
    return 0;
}
