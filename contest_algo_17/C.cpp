#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "BigIntegerAlgorithms.hh"

std::unordered_map<int, BigUnsigned> factorials;
void initFactorialsReversed(int value) {
    factorials[0] = 1;
    BigUnsigned big(1);
    for (int i = 1; i <= value; ++i) {
        big *= i;
        factorials[i] = big;
    }
}

class Permutations {
private:
    int n_objects_;
    std::vector<int> per_;

    void print() {
        for (const int digit : per_) {
            std::cout << digit + 1 << " ";
        }
        std::cout << "\n";
    }

public:
    explicit Permutations(int n_objects) {
        n_objects_ = n_objects;
        per_.resize(n_objects_);
    }

    void printPermutationByIndex(BigUnsigned permutation_i) {
        initFactorials(n_objects_ - 1);
        BigUnsigned perm_left = permutation_i;
        std::unordered_set<int> used;
        BigUnsigned digits_passed;
        BigUnsigned next_permutations;
        for (int place = 0; place < n_objects_; ++place) {
            next_permutations = factorials[n_objects_ - 1 - place];
            digits_passed = perm_left / next_permutations;
            perm_left -= digits_passed * next_permutations;
            int digit = 0;
            bool inf_loop = true;
            while (inf_loop) {
                if (used.count(digit)) {
                    ++digit;
                } else if (digits_passed != 0) {
                    ++digit;
                    --digits_passed;
                } else {
                    break;
                }
            }
            used.insert(digit);
            per_[place] = digit;
        }
        print();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_students;
    std::string str_permutation_i;
    std::cin >> n_students >> str_permutation_i;
    Permutations algo(n_students);
    algo.printPermutationByIndex(stringToBigUnsigned(str_permutation_i) - 1);
    return 0;
}
