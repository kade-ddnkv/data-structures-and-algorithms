#include <iostream>
#include <vector>

int findNextCityToRelocate(const std::vector<int> &cities, int city_index) {
    for (size_t i = city_index + 1; i < cities.size(); ++i) {
        if (cities[i] < cities[city_index]) {
            return i;
        }
    }
    return -1;
}

void printRelocation(const std::vector<int> &cities) {
    for (size_t i = 0; i < cities.size(); ++i) {
        std::cout << findNextCityToRelocate(cities, i) << " ";
    }
}

std::vector<int> readCitiesFromConsole() {
    std::vector<int> cities;
    int cities_number;
    int curr_city;
    std::cin >> cities_number;
    cities.reserve(cities_number);

    for (int i = 0; i < cities_number; ++i) {
        std::cin >> curr_city;
        cities.emplace_back(curr_city);
    }
    return cities;
}

int main() {
    std::vector<int> cities = readCitiesFromConsole();
    printRelocation(cities);
    return 0;
}
