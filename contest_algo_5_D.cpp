#include <iostream>
#include <vector>
#include <utility>

struct Place {
    int distance;
    int number;
};

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

std::vector<Place> readVectorOfPlacesFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<Place> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i].distance;
        elements[i].number = i + 1;
    }
    return elements;
}

void siftDownPlaces(std::vector<Place> *data, int root_index, int right_border) {
    int root = root_index;
    int left, right, max;
    while (root * 2 + 1 <= right_border) {
        left = root * 2 + 1;
        right = root * 2 + 2;
        max = root;
        if ((*data)[max].distance < (*data)[left].distance) {
            max = left;
        }
        if (right <= right_border && (*data)[max].distance < (*data)[right].distance) {
            max = right;
        }
        if (max == root) {
            return;
        }
        std::swap((*data)[root], (*data)[max]);
        root = max;
    }
}

void makeHeapPlaces(std::vector<Place> *data) {
    for (int i = (data->size() - 1 - 1) / 2; i >= 0; --i) {
        siftDownPlaces(data, i, data->size() - 1);
    }
}

void heapSortPlaces(std::vector<Place> *data) {
    makeHeapPlaces(data);
    int heap_size = data->size();
    while (heap_size != 0) {
        std::swap((*data)[0], (*data)[heap_size - 1]);
        --heap_size;
        siftDownPlaces(data, 0, heap_size - 1);
    }
}

// Возвращает первый элемент, больший или равный запросу.
// Если такого элемента нет, возвращает data.size().
int upperBoundInclusive(const std::vector<Place> &data, int query) {
    int left = -1;
    int right = data.size();
    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (data[mid].distance < query) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

void printNearestShelters(const std::vector<int> &villages, const std::vector<Place> &shelters) {
    int upper_bound;
    for (const int &curr_village_distance : villages) {
        // upper_bound - индекс первого убежища, >= по дистанции.
        upper_bound = upperBoundInclusive(shelters, curr_village_distance);
        if (upper_bound == 0) {
            std::cout << shelters[upper_bound].number << " ";
        } else if (upper_bound == static_cast<int>(shelters.size())) {
            std::cout << shelters[upper_bound - 1].number << " ";
        } else if (curr_village_distance - shelters[upper_bound - 1].distance <=
                   shelters[upper_bound].distance - curr_village_distance) {
            std::cout << shelters[upper_bound - 1].number << " ";
        } else {
            std::cout << shelters[upper_bound].number << " ";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<int> villages = readVectorFromConsole();
    std::vector<Place> shelters = readVectorOfPlacesFromConsole();
    heapSortPlaces(&shelters);
    printNearestShelters(villages, shelters);
    return 0;
}
