#include <iostream>

int getMaxPossibleDefeats() {
    int r1, s1, p1;
    int r2, s2, p2;
    std::cin >> r1 >> s1 >> p1;
    std::cin >> r2 >> s2 >> p2;

    int max_defeats_count = 0;
    max_defeats_count += r1 > s2 ? s2 : r1;
    max_defeats_count += s1 > p2 ? p2 : s1;
    max_defeats_count += p1 > r2 ? r2 : p1;
    return max_defeats_count;
}

int main() {
    std::cout << getMaxPossibleDefeats();
    return 0;
}
