#include <iostream>

void readAndPrintFirstSecondMaxGnomes() {
    int current_man;
    int first_max = 0;
    int second_max = 0;
    do {
        std::cin >> current_man;
        if (current_man > first_max) {
            second_max = first_max;
            first_max = current_man;
        } else if (current_man > second_max) {
            second_max = current_man;
        }
    } while (current_man != 0);
    std::cout << first_max << "\n" << second_max;
}

int main() {
    readAndPrintFirstSecondMaxGnomes();

    return 0;
}
