#include <string>
#include <vector>
#include <algorithm>
#include "BigIntegerAlgorithms.hh"

void printPermutationByIndex(int n_objects, BigUnsigned perm_left) {
    int after = std::min(n_objects, 1000);
    int before = n_objects - after;
    for (int i = 0; i < before; ++i) {
        std::cout << 1 << " ";
    }
    std::vector<int> perm(after);
    for (int place = n_objects - 1; place >= before; --place) {
        perm[place - before] = std::stoi(bigUnsignedToString(perm_left % (place + 1)));
        perm_left /= place + 1;
    }
    for (int i = 0; i < after - 1; ++i) {
        std::cout << perm[i] + 1 << " ";
    }
    std::cout << perm.back() + 1 << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_days;
    std::string str_permutation_i;
    std::cin >> n_days >> str_permutation_i;
    printPermutationByIndex(n_days, stringToBigUnsigned(str_permutation_i) - 1);
    return 0;
}
