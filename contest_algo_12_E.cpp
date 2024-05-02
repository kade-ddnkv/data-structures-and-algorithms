#include <iostream>
#include <vector>
#include <algorithm>

struct Object {
    int weight;
    int price;

    bool operator<(const Object &other) {
        return (1.0 * price / weight) < (1.0 * other.price / other.weight);
    }
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

std::vector<Object> fillBackpack(int number_of_objects, int max_weight,
                                 std::vector<Object> all_objs) {
    std::vector<Object> backpack;
    std::sort(all_objs.begin(), all_objs.end());
    std::reverse(all_objs.begin(), all_objs.end());
    int index = 0;
    int weight_left = max_weight;
    while (index < number_of_objects && weight_left != 0) {
        if (all_objs[index].weight <= weight_left) {
            backpack.push_back(all_objs[index]);
            weight_left -= all_objs[index].weight;
        }
        ++index;
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
    int number_of_objects;
    int max_weight;
    std::vector<Object> all_objs;
    readData(&number_of_objects, &max_weight, &all_objs);
    std::vector<Object> backpack = fillBackpack(number_of_objects, max_weight, all_objs);
    printResults(backpack);
    return 0;
}
