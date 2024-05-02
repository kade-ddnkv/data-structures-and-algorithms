#include <iostream>
#include <vector>
#include <algorithm>

struct Drone {
    int id;
    int load_capacity;
};

std::vector<Drone> readDronesFromConsole() {
    int number_of_drones;
    std::cin >> number_of_drones;
    std::vector<Drone> drones(number_of_drones);
    int curr_id, curr_load_capacity;
    for (int i = 0; i < number_of_drones; ++i) {
        std::cin >> curr_id >> curr_load_capacity;
        drones[i].id = curr_id;
        drones[i].load_capacity = curr_load_capacity;
    }
    return drones;
}

int getDigit(int number, int digit_place) {
    int mask = 255;
    return mask & (number >> (digit_place * 8));
}

std::vector<Drone> tmp_elements_copy;

void msdRadixSort(std::vector<Drone> *data, int left, int right, int digit_place = 3) {
    if (digit_place < 0 || left >= right) {
        return;
    }
    int curr_digit;
    int max_digit = -1;
    for (int i = left; i <= right; ++i) {
        curr_digit = getDigit((*data)[i].load_capacity, digit_place);
        if (curr_digit > max_digit) {
            max_digit = curr_digit;
        }
    }
    std::vector<int> counts(max_digit + 1);
    for (int i = left; i <= right; ++i) {
        curr_digit = getDigit((*data)[i].load_capacity, digit_place);
        ++counts[curr_digit];
    }
    std::vector<int> recursive_calls;
    recursive_calls.push_back(counts[counts.size() - 1]);
    for (int i = counts.size() - 2; i >= 0; --i) {
        counts[i] += counts[i + 1];
        if (counts[i] != counts[i + 1]) {
            recursive_calls.push_back(counts[i]);
        }
    }
    for (int i = right; i >= left; --i) {
        curr_digit = getDigit((*data)[i].load_capacity, digit_place);
        --counts[curr_digit];
        tmp_elements_copy[left + counts[curr_digit]] = (*data)[i];
    }
    for (int i = left; i <= right; ++i) {
        (*data)[i] = tmp_elements_copy[i];
    }
    msdRadixSort(data, left, left + recursive_calls[0] - 1, digit_place - 1);
    for (size_t i = 1; i < recursive_calls.size(); ++i) {
        msdRadixSort(data, left + recursive_calls[i - 1], left + recursive_calls[i] - 1,
                     digit_place - 1);
    }
}

int main() {
    std::vector<Drone> drones = readDronesFromConsole();
    tmp_elements_copy.resize(drones.size());
    msdRadixSort(&drones, 0, drones.size() - 1);
    for (const Drone &drone : drones) {
        std::cout << drone.id << "\t" << drone.load_capacity << "\n";
    }
    return 0;
}
