#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

std::tuple<int, int> getMinMaxDiscomfortNumbers(const std::vector<int> &pupils, int inside_squad) {
    int min = pupils[inside_squad - 1] - pupils[0];
    int max = min;
    int curr_discomfort_number;
    for (int i = 1; i < pupils.size() - inside_squad + 1; ++i) {
        curr_discomfort_number = pupils[i + inside_squad - 1] - pupils[i];
        min = std::min(min, curr_discomfort_number);
        max = std::max(max, curr_discomfort_number);
    }
    return {min, max};
}

bool tryDividePupilsIntoSquads(const std::vector<int> &pupils, int number_of_squads,
                               int inside_squad, int max_discomfort) {
    int index = 0;
    while (number_of_squads != 0) {
        if (index + inside_squad - 1 > pupils.size() - 1) {
            return false;
        }
        if (pupils[index + inside_squad - 1] - pupils[index] <= max_discomfort) {
            index += inside_squad;
            --number_of_squads;
        } else {
            ++index;
        }
    }
    return true;
}

int findMinDiscomfortNumber(const std::vector<int> &pupils, int number_of_squads,
                            int inside_squad) {
    auto[left, right] = getMinMaxDiscomfortNumbers(pupils, inside_squad);
    left -= 1;
    right += 1;
    int curr_discomfort_number;
    while (right - left > 1) {
        curr_discomfort_number = left + (right - left) / 2;
        if (tryDividePupilsIntoSquads(pupils, number_of_squads, inside_squad,
                                      curr_discomfort_number)) {
            right = curr_discomfort_number;
        } else {
            left = curr_discomfort_number;
        }
    }
    return right;
}

int main() {
    int number_of_pupils;
    int number_of_squads;
    int inside_squad;

    std::cin >> number_of_pupils >> number_of_squads >> inside_squad;
    std::vector<int> pupils(number_of_pupils);
    for (int i = 0; i < number_of_pupils; ++i) {
        std::cin >> pupils[i];
    }
    std::sort(pupils.begin(), pupils.end());

    std::cout << findMinDiscomfortNumber(pupils, number_of_squads, inside_squad);
    return 0;
}
