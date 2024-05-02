#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <functional>

class Erlich {
private:
    const int k_max_number_ = 12;
    int max_number_;
    // multi-partition
    std::vector<int> mp_;

    std::vector<int> perm_;
    std::vector<std::string> combinations_;

    int n_places_;

    bool initPermutation() {
        int count = 0;
        for (const int &digit_count : mp_) {
            count += digit_count;
        }
        if (count > n_places_) {
            return false;
        }
        int index = n_places_ - count;
        for (int i = 0; i < index; ++i) {
            perm_[i] = 0;
        }
        for (size_t i = 0; i < mp_.size(); ++i) {
            for (int j = 0; j < mp_[i]; ++j) {
                perm_[index++] = i;
            }
        }
        return true;
    }

    bool nextPermutation() {
        int i = n_places_ - 2;
        while (i != -1 && perm_[i] >= perm_[i + 1]) {
            --i;
        }
        if (i == -1) {
            return false;
        }
        int j = n_places_ - 1;
        while (perm_[i] >= perm_[j]) {
            --j;
        }
        std::swap(perm_[i], perm_[j]);
        int l = i + 1, r = n_places_ - 1;
        while (l < r) {
            std::swap(perm_[l++], perm_[r--]);
        }
        return true;
    }

    void savePermutation() {
        std::string str;
        for (int i = 0; i < n_places_ - 1; ++i) {
            str += std::to_string(perm_[i]) + " ";
        }
        str += std::to_string(perm_.back());
        combinations_.push_back(str);
    }

    void print() {
        for (int i = mp_.size() - 1; i >= 0; --i) {
            for (int j = 0; j < mp_[i]; ++j) {
                if (i == 0 && j == mp_[i] - 1) {
                    std::cout << i;
                } else {
                    std::cout << i << " ";
                }
            }
        }
        std::cout << "\n";
    }

public:
    explicit Erlich(int n_places) {
        n_places_ = n_places;
        perm_.resize(n_places_);
    }

    void generateAllCombinations(int number) {
        max_number_ = std::min(number, k_max_number_);
        mp_.resize(max_number_ + 1);
        mp_[1] = number;
        if (initPermutation()) {
            savePermutation();
        }
        bool inf_loop = true;
        int plus_ones;
        while (inf_loop) {
            int smallest = 0;
            while (mp_[smallest] == 0) {
                ++smallest;
            }
            if (smallest == max_number_) {
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
                if (next_smallest == max_number_) {
                    break;
                }
                plus_ones =
                    smallest * (mp_[smallest]) + next_smallest * (mp_[next_smallest] - 1) - 1;
                mp_[smallest] = 0;
                mp_[next_smallest] = 0;
                ++mp_[next_smallest + 1];
                mp_[1] += plus_ones;
            }
            if (initPermutation()) {
                savePermutation();
                while (nextPermutation()) {
                    savePermutation();
                }
            }
        }
    }

    void printAllCombinations() {
        std::sort(combinations_.begin(), combinations_.end(), std::greater<std::string>());
        for (const std::string &comb : combinations_) {
            std::cout << comb << "\n";
        }
    }

    void printCombination(int combination_index) {
        std::sort(combinations_.begin(), combinations_.end(), std::greater<std::string>());
        std::cout << combinations_[combination_index];
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_students, n_pizzas, combination_number;
    std::cin >> n_students >> n_pizzas >> combination_number;
    Erlich algo(n_pizzas);
    algo.generateAllCombinations(n_students);
    algo.printCombination(combination_number - 1);
    return 0;
}
