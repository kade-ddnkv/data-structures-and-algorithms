#include <vector>
#include <utility>
#include "./student-ski.h"

template <typename T, typename F>
void mergeLists(std::vector<T> *list, std::vector<T> *tmp_list_for_merging, int start1, int end1,
                int start2, int end2, F comparator) {
    int index1 = start1;
    int index2 = start2;
    int index_for_merged = 0;
    while (index1 <= end1 && index2 <= end2) {
        if (comparator((*list)[index1], (*list)[index2]) < 0) {
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

template <typename T, typename F>
void mergeSortRecursive(std::vector<T> *list, std::vector<T> *tmp_list_for_merging, int start,
                        int end, F comparator) {
    if (start < end) {
        int mid = start + (end - start) / 2;
        mergeSortRecursive(list, tmp_list_for_merging, start, mid, comparator);
        mergeSortRecursive(list, tmp_list_for_merging, mid + 1, end, comparator);
        mergeLists(list, tmp_list_for_merging, start, mid, mid + 1, end, comparator);
    }
}

template <typename T, typename F>
void mergeSort(std::vector<T> *list, F comparator) {
    std::vector<T> tmp_list_for_merging(list->size());
    mergeSortRecursive(list, &tmp_list_for_merging, 0, list->size() - 1, comparator);
}

int studentComparatorById(const Student &s1, const Student &s2) {
    if (s1.id < s2.id) {
        return -1;
    } else if (s1.id > s2.id) {
        return 1;
    } else {
        return 0;
    }
}

void findSkiForStudent(const Student &student, std::vector<Ski> *skis) {
    int index = student.id - 1;
    while (compare(student, (*skis)[index]) != 0) {
        ++index;
    }
    std::swap((*skis)[student.id - 1], (*skis)[index]);
}

void findPairs(std::vector<Student> *students, std::vector<Ski> *skis) {
    mergeSort(students, studentComparatorById);
    for (const Student &student : *students) {
        findSkiForStudent(student, skis);
    }
}
