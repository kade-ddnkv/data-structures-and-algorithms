#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Interval {
    double begin;
    double end;
    double weight;
};

struct ComparerByIntervalBeginning {
    bool operator()(const Interval& interval1, const Interval& interval2) {
        return (interval1.begin < interval2.begin);
    }
};

void readData(std::vector<Interval>* intervals) {
    int number_of_intervals;
    std::cin >> number_of_intervals;
    intervals->resize(number_of_intervals);
    for (int i = 0; i < number_of_intervals; ++i) {
        std::cin >> (*intervals)[i].begin >> (*intervals)[i].end >> (*intervals)[i].weight;
    }
}

struct SubtaskResult {
    double last_interval_end;
    double sum_weight;
    SubtaskResult(double last_interval_end, double sum_weight) {
        this->last_interval_end = last_interval_end;
        this->sum_weight = sum_weight;
    }
};

struct ComparerBySumWeight {
    bool operator()(const SubtaskResult& subtask1, const SubtaskResult& subtask2) {
        return (subtask1.sum_weight < subtask2.sum_weight);
    }
};

double getMaxWeightOfNonOverlappingIntervals(std::vector<Interval> intervals) {
    std::sort(intervals.begin(), intervals.end(), ComparerByIntervalBeginning());
    std::vector<SubtaskResult> subtasks;

    for (const Interval& interval : intervals) {
        // Среди подзадач, у которых последний интервал заканчивается раньше начала текущего,
        // нахожу максимальный вес.
        double max_weight_before = 0;
        for (const SubtaskResult& subtask : subtasks) {
            if (subtask.last_interval_end <= interval.begin &&
                subtask.sum_weight > max_weight_before) {
                max_weight_before = subtask.sum_weight;
            }
        }
        if (max_weight_before + interval.weight <= max_weight_before) {
            throw std::exception();
        }
        subtasks.push_back(SubtaskResult(interval.end, max_weight_before + interval.weight));
    }
    auto it_max_sum_weight =
        std::max_element(subtasks.begin(), subtasks.end(), ComparerBySumWeight());
    if (it_max_sum_weight == subtasks.end()) {
        throw std::exception();
    }
    if (it_max_sum_weight->sum_weight <= 0) {
        throw std::exception();
    }
    return it_max_sum_weight->sum_weight;
}

int main() {
    std::vector<Interval> intervals;
    readData(&intervals);
    std::cout << std::fixed << std::setprecision(8)
              << getMaxWeightOfNonOverlappingIntervals(intervals);
    return 0;
}
