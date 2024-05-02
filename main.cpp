#include <iostream>
#include <vector>

int main() {
    size_t n;
    int b;
    std::cin >> n;
    int *data = new int[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> data[i];
    }
    return 0;
}
