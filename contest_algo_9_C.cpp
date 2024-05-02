#include <iostream>
#include <string>
#include <cmath>

std::string readString() {
    std::string str;
    std::cin >> str;
    return str;
}

int getNumberOfControlBits(int str_len) {
    int control_bits = log2(str_len + 1);
    while (control_bits < log2(control_bits + str_len + 1)) {
        ++control_bits;
    }
    return control_bits;
}

std::string hammingEncode(std::string str) {
    int number_of_control_bits = getNumberOfControlBits(str.length());
    for (int i = 0; i < number_of_control_bits; ++i) {
        str.insert(pow(2, i) - 1, "0");
    }
    int new_str_len = str.length();
    for (int curr_degree = 0, powered_degree = 1; curr_degree < number_of_control_bits;
         ++curr_degree, powered_degree *= 2) {
        // Вычислить контрольный бит.
        int sum = 0;
        int local_index = powered_degree;
        while (local_index <= new_str_len) {
            int bit = (local_index - powered_degree) & (1 << curr_degree);
            if (bit == 0) {
                sum += str[local_index - 1] - '0';
            }
            ++local_index;
        }
        // Установить контрольный бит.
        if (sum % 2 == 1) {
            str[powered_degree - 1] = '1';
        }
    }
    return str;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string str_to_encode = readString();
    std::cout << hammingEncode(str_to_encode);
    return 0;
}
