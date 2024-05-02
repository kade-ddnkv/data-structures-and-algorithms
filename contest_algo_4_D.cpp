#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

std::unordered_map<int, int> readKeysMapFromConsole() {
    int number_of_chars;
    std::cin >> number_of_chars;
    std::unordered_map<int, int> chars_frequency;
    int curr_char;
    getchar();
    for (int i = 0; i < number_of_chars; ++i) {
        curr_char = getchar();
        chars_frequency[curr_char] += 1;
    }
    return chars_frequency;
}

std::string createPalindrome(const std::unordered_map<int, int> &chars_frequency) {
    std::vector<int> doubled_chars(280);
    std::vector<int> odd_chars(280);
    for (auto pair : chars_frequency) {
        doubled_chars[pair.first] = pair.second / 2;
        if (pair.second % 2 != 0) {
            odd_chars[pair.first] = 1;
        }
    }
    std::string palindrome;
    for (size_t i = 0; i < doubled_chars.size(); ++i) {
        for (int j = 0; j < doubled_chars[i]; ++j) {
            palindrome.push_back(i);
        }
    }
    size_t first_odd_char_index = 0;
    while (first_odd_char_index < odd_chars.size() && odd_chars[first_odd_char_index] == 0) {
        ++first_odd_char_index;
    }
    if (first_odd_char_index < odd_chars.size()) {
        palindrome.push_back(first_odd_char_index);
    }
    for (int i = doubled_chars.size() - 1; i >= 0; --i) {
        for (int j = 0; j < doubled_chars[i]; ++j) {
            palindrome.push_back(i);
        }
    }
    return palindrome;
}

int main() {
    std::unordered_map<int, int> chars_frequency = readKeysMapFromConsole();
    std::string palindrome = createPalindrome(chars_frequency);
    std::cout << palindrome;
    return 0;
}
