#include <iostream>
#include <vector>
#include <algorithm>

class Erlich {
private:
    const int k_max_tent_cap_ = 12;
    // multi-partition
    std::vector<int> mp_;

    void print(bool is_first_line) {
        if (!is_first_line) {
            std::cout << "\n";
        }
        for (int i = mp_.size() - 1; i >= 0; --i) {
            for (int j = 0; j < mp_[i]; ++j) {
                if (i == 0 && j == mp_[i] - 1) {
                    std::cout << i;
                } else {
                    std::cout << i << " ";
                }
            }
        }
    }

public:
    void printAllPartitions(int number) {
        int max_number = std::min(number, k_max_tent_cap_);
        mp_.resize(max_number + 1);
        mp_[1] = number;
        print(true);
        bool inf_loop = true;
        int plus_ones;
        while (inf_loop) {
            int smallest = 0;
            while (mp_[smallest] == 0) {
                ++smallest;
            }
            if (smallest == max_number) {
                break;
            }
            if (mp_[smallest] > 1) {
                plus_ones = smallest * (mp_[smallest] - 1) - 1;
                mp_[smallest] = 0;
                ++mp_[smallest + 1];
                mp_[1] += plus_ones;
            } else {
                int next_smallest = smallest + 1;
                while (mp_[next_smallest] == 0) {
                    ++next_smallest;
                }
                if (next_smallest == max_number) {
                    break;
                }
                plus_ones =
                    smallest * (mp_[smallest]) + next_smallest * (mp_[next_smallest] - 1) - 1;
                mp_[smallest] = 0;
                mp_[next_smallest] = 0;
                ++mp_[next_smallest + 1];
                mp_[1] += plus_ones;
            }
            print(false);
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int number;
    std::cin >> number;
    Erlich algo;
    algo.printAllPartitions(number);
    return 0;
}
