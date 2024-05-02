#include "BigIntegerUtils2.h"

BigUnsigned factorial(int value) {
    BigUnsigned big(1);
    for (int i = 2; i <= value; ++i) {
        big *= i;
    }
    return big;
}

BigUnsigned combinations(int n_objects, int n_places) {
    return factorial(n_objects) / (factorial(n_objects - n_places) * factorial(n_places));
}

BigUnsigned pow(int number, int to_pow) {
    BigUnsigned big(1);
    for (int i = 0; i < to_pow; ++i) {
        big *= number;
    }
    return big;
}

BigUnsigned waysToSplitToPairs(int n_people) {
    int n_pairs = n_people / 2;
    return factorial(n_people) / (pow(2, n_pairs) * factorial(n_pairs));
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_students, n_places;
    std::cin >> n_students >> n_places;

    std::cout << factorial(n_students) << "\n";
    std::cout << combinations(n_students, n_places) << "\n";
    std::cout << waysToSplitToPairs(n_students) << "\n";
    return 0;
}
