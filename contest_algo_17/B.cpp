#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
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

class Variations {
private:
    int n_objects_, n_places_;
    std::vector<int> variation_;

    void print() {
        for (const int digit : variation_) {
            std::cout << digit + 1 << " ";
        }
        std::cout << "\n";
    }

public:
    Variations(int n_objects, int n_places) {
        n_objects_ = n_objects;
        n_places_ = n_places;
        variation_.resize(n_places_);
    }

    void printVariationByIndex(BigUnsigned variation_i) {
        initFactorials(n_objects_ - 1);
        BigUnsigned vars_left = variation_i;
        std::unordered_set<int> used;
        BigUnsigned digits_passed;
        BigUnsigned next_variations;
        for (int place = 0; place < n_places_; ++place) {
            next_variations =
                factorials[n_objects_ - 1 - place] / factorials[n_objects_ - n_places_];
            digits_passed = vars_left / next_variations;
            vars_left -= digits_passed * next_variations;
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
            variation_[place] = digit;
        }
        print();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_shirts, n_days;
    std::string str_variation_i;
    std::cin >> n_shirts >> n_days >> str_variation_i;
    Variations algo(n_shirts, n_days);
    algo.printVariationByIndex(stringToBigUnsigned(str_variation_i) - 1);
    return 0;
}
