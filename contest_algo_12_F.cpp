#include <iostream>
#include <vector>
#include <algorithm>

struct Object {
    int weight;
    int price;
};

void readData(int *number_of_objects, int *max_weight, std::vector<Object> *all_objs) {
    std::cin >> *number_of_objects >> *max_weight;
    all_objs->resize(*number_of_objects);
    for (int i = 0; i < *number_of_objects; ++i) {
        std::cin >> (*all_objs)[i].weight;
    }
    for (int i = 0; i < *number_of_objects; ++i) {
        std::cin >> (*all_objs)[i].price;
    }
}

int computeSubtask(int number_of_objects, int max_weight,
                   std::vector<std::vector<int64_t>> *subtasks,
                   const std::vector<Object> &all_objs) {
    if ((*subtasks)[number_of_objects][max_weight] != -1) {
        return (*subtasks)[number_of_objects][max_weight];
    }

    int result;
    if (all_objs[number_of_objects - 1].weight > max_weight) {
        result = computeSubtask(number_of_objects - 1, max_weight, subtasks, all_objs);
    } else {
        result = std::max(computeSubtask(number_of_objects - 1, max_weight, subtasks, all_objs),
                          computeSubtask(number_of_objects - 1,
                                         max_weight - all_objs[number_of_objects - 1].weight,
                                         subtasks, all_objs) +
                              all_objs[number_of_objects - 1].price);
    }
    (*subtasks)[number_of_objects][max_weight] = result;
    return result;
}

std::vector<Object> fillBackpackDynamicOptimal(int number_of_objects, int max_weight,
                                               std::vector<Object> all_objs) {
    std::vector<std::vector<int64_t>> subtasks;
    subtasks.resize(number_of_objects + 1);
    subtasks[0].resize(max_weight + 1, 0);
    for (int i = 1; i < number_of_objects + 1; ++i) {
        subtasks[i].resize(max_weight + 1, -1);
        subtasks[i][0] = 0;
    }
    computeSubtask(number_of_objects, max_weight, &subtasks, all_objs);

    // Восстановление пути из правого нижнего угла таблицы.
    std::vector<Object> backpack;
    int object_i = number_of_objects;
    int weight_i = max_weight;
    while (subtasks[object_i][weight_i] != 0) {
        if (subtasks[object_i][weight_i] == subtasks[object_i - 1][weight_i]) {
            --object_i;
        } else {
            --object_i;
            backpack.push_back(all_objs[object_i]);
            weight_i -= all_objs[object_i].weight;
        }
    }
    return backpack;
}

void printResults(const std::vector<Object> &backpack) {
    int64_t sum_cost = 0;
    int64_t sum_weight = 0;
    for (size_t i = 0; i < backpack.size(); ++i) {
        sum_cost += backpack[i].price;
        sum_weight += backpack[i].weight;
    }

    std::cout << sum_cost << "\n";
    std::cout << sum_weight << "\n";
    std::cout << backpack.size() << "\n";
    for (size_t i = 0; i < backpack.size(); ++i) {
        std::cout << backpack[i].weight << " ";
    }
    std::cout << "\n";
    for (size_t i = 0; i < backpack.size(); ++i) {
        std::cout << backpack[i].price << " ";
    }
    std::cout << "\n";
}

int main() {
    int number_of_objects, max_weight;
    std::vector<Object> all_objs;
    readData(&number_of_objects, &max_weight, &all_objs);
    std::vector<Object> backpack =
        fillBackpackDynamicOptimal(number_of_objects, max_weight, all_objs);
    printResults(backpack);
    return 0;
}
