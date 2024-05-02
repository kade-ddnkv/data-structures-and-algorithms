#include "BigIntegerAlgorithms.hh"

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

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int x, y;
    std::cin >> x >> y;
    std::cout << combinations(x + y, x);
    return 0;
}
