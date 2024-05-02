#include <iostream>
#include <cmath>
#include <iomanip>

double function(double var) {
    return pow(var, 2) + sqrt(var);
}

double bisectionSearch(double answer_c) {
    double left = 0 - 1;
    double right = 100000 + 1;
    double mid;
    while (right - left > 0.0000001) {
        mid = left + (right - left) / 2;
        if (function(mid) < answer_c) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right;
}

int main() {
    double equal_c;
    std::cin >> equal_c;
    std::cout << std::fixed << std::setprecision(6) << bisectionSearch(equal_c);
    return 0;
}
