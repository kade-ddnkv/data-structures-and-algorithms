#include <unordered_map>
#include <vector>
#include <string>
#include "BigIntegerAlgorithms.hh"

std::unordered_map<int, BigUnsigned> factorials;
void initFactorials(int value) {
    factorials[0] = 1;
    BigUnsigned big(1);
    for (int i = 1; i <= value; ++i) {
        big *= i;
        factorials[i] = big;
    }
}

class Combinations {
private:
    int n_objects_, n_places_;
    std::vector<int> comb_;

    void print() {
        for (const int digit : comb_) {
            std::cout << digit + 1 << " ";
        }
        std::cout << "\n";
    }

public:
    Combinations(int n_objects, int n_places) {
        n_objects_ = n_objects;
        n_places_ = n_places;
        comb_.resize(n_places_);
    }

    void printCombinationByIndex(BigUnsigned combination_i) {
        initFactorials(n_objects_ - 1);
        BigUnsigned comb_left = combination_i;
        int max_digit = -1;
        BigUnsigned next_placements;
        for (int place = 0; place < n_places_; ++place) {
            int digit = max_digit + 1;
            bool inf_loop = true;
            while (inf_loop) {
                // Количество соединений (комбинаций), которые исчезнут, если поставить
                // определенную цифру, теперь зависит и от текущей цифры.
                // Например:
                // Для 0 на первом месте: 3! / (2! * 1!) = 3
                // Для 1 на первом месте: 2! / (1! * 1!) = 2
                // Для 3 на первом месте: 1! / (0! * 1!) = 1
                next_placements = factorials[n_objects_ - digit - 1] /
                                  (factorials[(n_objects_ - digit - 1) - (n_places_ - 1 - place)] *
                                   factorials[n_places_ - 1 - place]);
                if (next_placements <= comb_left) {
                    ++digit;
                    comb_left -= next_placements;
                } else {
                    break;
                }
            }
            max_digit = digit;
            comb_[place] = digit;
        }
        print();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_tasks_all, n_tasks_in_day;
    std::string str_plan_i;
    std::cin >> n_tasks_all >> str_plan_i >> n_tasks_in_day;
    Combinations algo(n_tasks_all, n_tasks_in_day);
    algo.printCombinationByIndex(stringToBigUnsigned(str_plan_i) - 1);
    return 0;
}
