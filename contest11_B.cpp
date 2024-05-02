#include <iostream>

struct Edge {
    int left;
    int right;
    int weight;
};

class GraphDisjointSet {
    Edge** edges_;
    int edges_number_;
    int number_of_vertices_;

    // Массив для хранения ребер, полученных в первом поиске мин. ост. дерева.
    Edge** spanning_tree_edges_;
    int* parents_;
    int min_unites_number_;

public:
    GraphDisjointSet(Edge** edges, int edges_number, int number_of_vertices) {
        edges_ = edges;
        edges_number_ = edges_number;
        number_of_vertices_ = number_of_vertices;
        parents_ = new int[number_of_vertices];
        min_unites_number_ = number_of_vertices - 1;
        spanning_tree_edges_ = new Edge*[min_unites_number_];
    }

    // Функция, выводящая первые два минимальных веса остовных деревьев в данном графе.
    void printFirstTwoMinTreeWeights() {
        int first_weight = getMinTreeWeight();
        int second_weight = INT32_MAX;
        int curr_weight;
        // Удаляется каждое ребро из самого "легкого" случая.
        // И для такого графа находится остовное дерево с мин. весом (если возможно).
        // В second_weight записывается минимальное такое значение.
        for (int i = 0; i < min_unites_number_; ++i) {
            curr_weight = getMinTreeWeightWithIgnoredEdge(spanning_tree_edges_[i]);
            if (curr_weight <= second_weight) {
                second_weight = curr_weight;
            }
        }
        std::cout << first_weight << " " << second_weight;
    }

    int find(int elem) {
        if (parents_[elem] == elem) {
            return elem;
        } else {
            return find(parents_[elem]);
        }
    }

    // Функция для поиска остовного дерева с минимальным весом.
    int getMinTreeWeight() {
        // Очистка массива parents_.
        for (int i = 0; i < number_of_vertices_; ++i) {
            parents_[i] = i;
        }

        int sum_weight = 0;
        int edge_index = 0;
        int unites_count = 0;

        while (unites_count != min_unites_number_) {
            int left_head = find(edges_[edge_index]->left - 1);
            int right_head = find(edges_[edge_index]->right - 1);
            if (left_head != right_head) {
                parents_[right_head] = left_head;
                sum_weight += edges_[edge_index]->weight;
                // В массив сохраняются использованные ребра.
                spanning_tree_edges_[unites_count] = edges_[edge_index];
                ++unites_count;
            }
            ++edge_index;
        }
        return sum_weight;
    }

    // Функция для поиска остовного дерева с минимальным весом при игнорировании одного ребра.
    int getMinTreeWeightWithIgnoredEdge(Edge* ignored_edge) {
        // Очистка массива parents_.
        for (int i = 0; i < number_of_vertices_; ++i) {
            parents_[i] = i;
        }

        int sum_weight = 0;
        int edge_index = 0;
        int unites_count = 0;

        while (unites_count != min_unites_number_) {
            // Без игнорируемого ребра может не получиться дерево.
            // Тогда возвращается самое большое значение.
            if (edge_index == edges_number_) {
                return INT32_MAX;
            }
            if (edges_[edge_index] == ignored_edge) {
                ++edge_index;
                continue;
            }

            int left_head = find(edges_[edge_index]->left - 1);
            int right_head = find(edges_[edge_index]->right - 1);
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
        delete[] spanning_tree_edges_;
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
    GraphDisjointSet set(edges, edges_number, vertices_number);
    set.printFirstTwoMinTreeWeights();

    for (int i = 0; i < edges_number; ++i) {
        delete edges[i];
    }
    delete[] edges;
    return 0;
}
