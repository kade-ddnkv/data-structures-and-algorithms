#include <iostream>
#include <string>
#include <vector>

std::vector<int> getPrefixArray(std::string str) {
    std::vector<int> prefix_array(str.size());
    prefix_array[0] = 0;
    int p_rec_res;
    int str_size = str.size();
    for (int i = 0; i < str_size - 1; ++i) {
        p_rec_res = prefix_array[i];
        while (p_rec_res > 0 && str[i + 1] != str[p_rec_res]) {
            p_rec_res = prefix_array[p_rec_res - 1];
        }
        if (str[i + 1] == str[p_rec_res]) {
            ++p_rec_res;
        }
        prefix_array[i + 1] = p_rec_res;
    }
    return prefix_array;
}

int main() {
    std::string str;
    std::cin >> str;
    std::vector<int> prefix_array = getPrefixArray(str);
    for (const int &elem : prefix_array) {
        std::cout << elem << " ";
    }
    return 0;
}
