#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <fstream>

std::vector<int> readVector(size_t max_count) {
    std::vector<int> result;
    result.reserve(max_count);
    for (size_t i = 0; i < max_count; ++i) {
        int elem;
        if (!(std::cin >> elem)) {
            break;
        }
        result.push_back(elem);
    }
    return result;
}

std::vector<int> readVectorFromFile(size_t max_count) {
    std::ifstream in("crashme_algo_6_D_out");
    std::vector<int> result;
    result.reserve(max_count);
    for (size_t i = 0; i < max_count; ++i) {
        int elem;
        if (!(in >> elem)) {
            break;
        }
        result.push_back(elem);
    }
    return result;
}

int main() {
    const size_t max_count = 32768u;
    auto data = readVectorFromFile(max_count);
    auto start_time = std::clock();

    std::sort(data.begin(), data.end());

    auto end_time = std::clock();
    double spent = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << spent << " seconds\n";

    if (!std::is_sorted(data.begin(), data.end())) {
        std::cout << "mmmm...\n";
        return 1;
    }

    if (spent > 0.04) {
        std::cout << "Shit happens\n";
        return 1;
    }
    return 0;
}