#include <iostream>
#include <vector>

std::vector<int> readVectorFromConsole() {
    int number_of_elements;
    std::cin >> number_of_elements;
    std::vector<int> elements(number_of_elements);
    for (int i = 0; i < number_of_elements; ++i) {
        std::cin >> elements[i];
    }
    return elements;
}

void mergeLists(std::vector<int> *list, std::vector<int> *tmp_list_for_merging, int start1,
                int end1, int start2, int end2) {
    int index1 = start1;
    int index2 = start2;
    int index_for_merged = 0;
    while (index1 <= end1 && index2 <= end2) {
        if ((*list)[index1] < (*list)[index2]) {
            (*tmp_list_for_merging)[index_for_merged++] = (*list)[index1++];
        } else {
            (*tmp_list_for_merging)[index_for_merged++] = (*list)[index2++];
        }
    }
    while (index1 <= end1) {
        (*tmp_list_for_merging)[index_for_merged++] = (*list)[index1++];
    }
    while (index2 <= end2) {
        (*tmp_list_for_merging)[index_for_merged++] = (*list)[index2++];
    }
    for (int i = 0; i < index_for_merged; ++i) {
        (*list)[i + start1] = (*tmp_list_for_merging)[i];
    }
}

void mergeSortRecursive(std::vector<int> *list, std::vector<int> *tmp_list_for_merging, int start,
                        int end) {
    if (start < end) {
        int mid = start + (end - start) / 2;
        mergeSortRecursive(list, tmp_list_for_merging, start, mid);
        mergeSortRecursive(list, tmp_list_for_merging, mid + 1, end);
        mergeLists(list, tmp_list_for_merging, start, mid, mid + 1, end);
    }
}

void mergeSort(std::vector<int> *list) {
    std::vector<int> tmp_list_for_merging(list->size());
    mergeSortRecursive(list, &tmp_list_for_merging, 0, list->size() - 1);
}

int main() {
    std::vector<int> list = readVectorFromConsole();
    mergeSort(&list);
    for (const int &element : list) {
        std::cout << element << " ";
    }
    return 0;
}
