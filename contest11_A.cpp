#include <iostream>

struct Edge {
    int left;
    int right;
    int weight;
};

class GraphDisjointSet {
    Edge** edges_;
    int* parents_;
    int min_unites_number_;

public:
    GraphDisjointSet(Edge** edges, int number_of_vertices) {
        edges_ = edges;
        parents_ = new int[number_of_vertices];
        for (int i = 0; i < number_of_vertices; ++i) {
            parents_[i] = i;
        }
        min_unites_number_ = number_of_vertices - 1;
    }

    int find(int elem) {
        if (parents_[elem] == elem) {
            return elem;
        } else {
            return find(parents_[elem]);
        }
    }

    int getMinTreeWeight() {
        int unites_count = 0;
        int sum_weight = 0;
        int edge_index = 0;

        // Пока не будет найдено минимальное остовное дерево.
        while (unites_count != min_unites_number_) {
            int left_head = find(edges_[edge_index]->left - 1);
            int right_head = find(edges_[edge_index]->right - 1);

            // Если узлы графа не соединены - соединить.
            if (left_head != right_head) {
                parents_[right_head] = left_head;
                sum_weight += edges_[edge_index]->weight;
                ++unites_count;
            }
            ++edge_index;
        }
        return sum_weight;
    }

    ~GraphDisjointSet() {
        delete[] parents_;
    }
};

// Функция для слияния двух отсортированных массивов.
void merge(Edge** array, int begin, int mid, int end) {
    int left_length = mid - begin + 1;
    int right_length = end - mid;

    Edge** left_array = new Edge*[left_length];
    Edge** right_array = new Edge*[right_length];

    for (int i = 0; i < left_length; ++i) {
        left_array[i] = array[begin + i];
    }
    for (int i = 0; i < right_length; ++i) {
        right_array[i] = array[mid + 1 + i];
    }

    int index_left = 0;
    int index_right = 0;
    int index_array = begin;
    while (index_left < left_length && index_right < right_length) {
        if (left_array[index_left]->weight <= right_array[index_right]->weight) {
            array[index_array] = left_array[index_left];
            ++index_left;
        } else {
            array[index_array] = right_array[index_right];
            ++index_right;
        }
        ++index_array;
    }

    while (index_left < left_length) {
        array[index_array] = left_array[index_left];
        ++index_left;
        ++index_array;
    }

    while (index_right < right_length) {
        array[index_array] = right_array[index_right];
        ++index_right;
        ++index_array;
    }

    delete[] left_array;
    delete[] right_array;
}

// Сортировка слиянием. O(N * log(N))
void mergeSort(Edge** array, int begin, int end) {
    if (begin >= end) {
        return;
    }
    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

Edge** readEdgesFromConsole(int edges_number) {
    Edge** edges = new Edge*[edges_number];
    for (int i = 0; i < edges_number; ++i) {
        edges[i] = new Edge();
        std::cin >> edges[i]->left;
        std::cin >> edges[i]->right;
        std::cin >> edges[i]->weight;
    }
    return edges;
}

int main() {
    int vertices_number;
    int edges_number;
    std::cin >> vertices_number >> edges_number;
    Edge** edges = readEdgesFromConsole(edges_number);
    mergeSort(edges, 0, edges_number - 1);
    GraphDisjointSet set(edges, vertices_number);
    std::cout << set.getMinTreeWeight();

    for (int i = 0; i < edges_number; ++i) {
        delete edges[i];
    }
    delete[] edges;
    return 0;
}
