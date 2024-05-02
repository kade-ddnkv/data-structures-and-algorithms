#include <iostream>
#include <string>
#include <vector>

// Сложность: O(n*log(n))
int getNumberOfPossibleK(std::string code) {
    int number_of_possible_k = 0;

    std::vector<int> prefix_xor(code.length());
    prefix_xor[0] = code[0] - '0';
    for (size_t i = 1; i < code.length(); ++i) {
        prefix_xor[i] = prefix_xor[i - 1] ^ (code[i] - '0');
    }

    // Для каждого k нужно проверить, что в массиве xor
    // все соответствующие контрольные биты равны 0.
    bool control_bits_are_zero;
    for (size_t k = 1; k <= code.length() - 1; ++k) {
        control_bits_are_zero = true;
        for (size_t i = k; i < code.length(); i += k + 1) {
            if (prefix_xor[i] != 0) {
                control_bits_are_zero = false;
                break;
            }
        }
        if (control_bits_are_zero) {
            ++number_of_possible_k;
        }
    }
    return number_of_possible_k;
}

int main() {
    int code_length;
    std::string code;
    std::cin >> code_length >> code;
    std::cout << getNumberOfPossibleK(code);
    return 0;
}
