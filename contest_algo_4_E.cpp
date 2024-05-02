#include <iostream>
#include <unordered_map>
#include <utility>

std::unordered_map<int, int> readKeysMapFromConsole() {
    int number_of_keys;
    std::cin >> number_of_keys;
    std::unordered_map<int, int> keys_endurance(number_of_keys);
    int curr_endurance;
    for (int i = 0; i < number_of_keys; ++i) {
        std::cin >> curr_endurance;
        keys_endurance.insert(std::make_pair(i, curr_endurance));
    }
    return keys_endurance;
}

void pressOnKeys(std::unordered_map<int, int> *keys_endurance) {
    int number_of_presses;
    std::cin >> number_of_presses;
    int curr_press;
    for (int i = 0; i < number_of_presses; ++i) {
        std::cin >> curr_press;
        --(*keys_endurance)[curr_press - 1];
    }
}

int main() {
    std::unordered_map<int, int> keys_endurance = readKeysMapFromConsole();
    pressOnKeys(&keys_endurance);
    for (size_t i = 0; i < keys_endurance.size(); ++i) {
        std::cout << (keys_endurance[i] >= 0 ? "no" : "yes") << "\n";
    }
    return 0;
}
