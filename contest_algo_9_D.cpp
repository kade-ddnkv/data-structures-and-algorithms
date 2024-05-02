#include <iostream>
#include <string>
#include <cmath>

std::string readString() {
    std::string str;
    std::cin >> str;
    return str;
}

int getNumberOfControlBits(int str_len) {
    int control_bits = log2(str_len);
    while (control_bits < log2(str_len)) {
        ++control_bits;
    }
    return control_bits;
}

std::string hammingDecode(std::string str) {
    int corrupted_bit_number = 0;
    int str_len = str.length();
    int number_of_control_bits = getNumberOfControlBits(str.length());

    for (int curr_degree = 0, powered_degree = 1; curr_degree < number_of_control_bits;
         ++curr_degree, powered_degree *= 2) {
        // Вычисление контрольного бита.
        int sum = 0;
        int local_index = powered_degree;
        while (local_index <= str_len) {
            int bit = (local_index - powered_degree) & (1 << curr_degree);
            if (bit == 0) {
                sum += str[local_index - 1] - '0';
            }
            ++local_index;
        }
        // Если вычисленный бит не совпал с переданным, то добавляется не ноль.
        corrupted_bit_number += powered_degree * (sum % 2);
    }
    // Если были несовпадающие контрольные биты.
    if (corrupted_bit_number != 0 && corrupted_bit_number - 1 < str_len) {
        str[corrupted_bit_number - 1] = ((str[corrupted_bit_number - 1] - '0') + 1) % 2 + '0';
    }
    // Удаление из строки контрольных битов.
    for (int i = pow(2, number_of_control_bits - 1); i >= 1; i /= 2) {
        str.erase(i - 1, 1);
    }
    return str;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string str_to_decode = readString();
    std::cout << hammingDecode(str_to_decode);
    return 0;
}
