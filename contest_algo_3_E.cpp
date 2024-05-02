#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <functional>

std::vector<int> readVectorFromConsoleLine() {
    std::string line;
    std::stringstream line_in;
    std::vector<int> elements;
    int curr_element;
    getline(std::cin, line);
    line_in << line;
    while (line_in >> curr_element) {
        elements.push_back(curr_element);
    }
    return elements;
}

void bubbleSortUpdated(std::vector<int> *data) {
    int upped_bound;
    int last_swap = data->size() - 1;
    do {
        upped_bound = last_swap;
        last_swap = 0;
        for (int i = 0; i < upped_bound; ++i) {
            if ((*data)[i] < (*data)[i + 1]) {
                std::swap((*data)[i], (*data)[i + 1]);
                last_swap = i;
            }
        }
    } while (last_swap != 0);
}

// Не хотел писать отдельную функцию сортировки Descending.
// Но если передавать в функцию сортировки параметр std::function<bool(int, int)> func ...
// ... например, std::less<int>() и std::greater<int>(), то возникает TL.
// Не могу понять, почему.
void bubbleSortUpdatedDescending(std::vector<int> *data) {
    int upped_bound;
    int last_swap = data->size() - 1;
    do {
        upped_bound = last_swap;
        last_swap = 0;
        for (int i = 0; i < upped_bound; ++i) {
            if ((*data)[i] > (*data)[i + 1]) {
                std::swap((*data)[i], (*data)[i + 1]);
                last_swap = i;
            }
        }
    } while (last_swap != 0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<int> distances = readVectorFromConsoleLine();
    std::vector<int> prices = readVectorFromConsoleLine();

    int64_t full_cost = 0;
    bubbleSortUpdated(&distances);
    bubbleSortUpdatedDescending(&prices);
    for (size_t i = 0; i < distances.size(); ++i) {
        full_cost += static_cast<int64_t>(distances[i]) * prices[i];
    }
    std::cout << full_cost;

    return 0;
}
