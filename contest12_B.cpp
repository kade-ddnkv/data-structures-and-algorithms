#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <functional>

std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> readNumbersFromConsole() {
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> queue;
    int numbers_count;
    std::cin >> numbers_count;
    int curr_number;
    for (int i = 0; i < numbers_count; ++i) {
        std::cin >> curr_number;
        queue.push(curr_number);
    }
    return queue;
}

int64_t getCalculationFee(
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> queue) {
    // Приоритетная очередь копируется.

    int number_of_operations = queue.size() - 1;

    int64_t calculation_fee = 0;
    int64_t local_sum;
    for (int i = 0; i < number_of_operations; ++i) {
        local_sum = queue.top();
        queue.pop();
        local_sum += queue.top();
        queue.pop();
        queue.push(local_sum);

        calculation_fee += local_sum;
    }

    return calculation_fee;
}

int main() {
    auto queue = readNumbersFromConsole();

    auto calculation_fee = getCalculationFee(queue);

    std::cout << std::fixed << std::setprecision(2) << calculation_fee * 0.05;

    return 0;
}
