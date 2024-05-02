#include <iostream>

void findIndexOfMaxInteractive(int number_of_elements) {
    int second_max_index;
    std::cout << "? " << 1 << " " << number_of_elements << "\n";
    std::cout.flush();
    std::cin >> second_max_index;

    int next_second_max_index;
    if (second_max_index == 1) {
        next_second_max_index = 2;
    } else if (second_max_index == number_of_elements) {
        next_second_max_index = number_of_elements;
    } else {
        std::cout << "? " << 1 << " " << second_max_index << "\n";
        std::cout.flush();
        std::cin >> next_second_max_index;
    }

    int left;
    int right;
    int mid;
    if (next_second_max_index == second_max_index) {
        left = 1;
        right = second_max_index;
        while (right - left > 1) {
            mid = left + (right - left) / 2;
            std::cout << "? " << mid << " " << second_max_index << "\n";
            std::cout.flush();
            std::cin >> next_second_max_index;
            if (next_second_max_index == second_max_index) {
                left = mid;
            } else {
                right = mid;
            }
        }
        std::cout << "! " << left;
    } else {
        left = second_max_index;
        right = number_of_elements;
        while (right - left > 1) {
            mid = left + (right - left) / 2 + ((right - left) % 2);
            std::cout << "? " << second_max_index << " " << mid << "\n";
            std::cout.flush();
            std::cin >> next_second_max_index;
            if (next_second_max_index == second_max_index) {
                right = mid;
            } else {
                left = mid;
            }
        }
        std::cout << "! " << right;
    }
}

int main() {
    int number_of_elements;
    std::cin >> number_of_elements;
    findIndexOfMaxInteractive(number_of_elements);
    return 0;
}
