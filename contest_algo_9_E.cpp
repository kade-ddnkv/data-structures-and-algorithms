#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::string readLine() {
    std::string line;
    getline(std::cin, line);
    return line;
}

const int kAlphabetSize = 128;

void initLzwMapper(std::unordered_map<std::string, int> *mapper) {
    for (int i = 0; i < kAlphabetSize; ++i) {
        (*mapper)[std::string(1, i)] = i;
    }
}

std::vector<int> lzwEncode(std::string str) {
    std::vector<int> encoded_sequence;
    std::string window;
    size_t next_char_i = 0;
    std::unordered_map<std::string, int> mapper;

    initLzwMapper(&mapper);
    // Проверять на окончание строки не надо, так как там стоит нуль-терминатор.
    while (next_char_i != str.length() + 1) {
        if (mapper.find(window + str[next_char_i]) != mapper.end()) {
            window += str[next_char_i];
        } else {
            encoded_sequence.push_back(mapper[window]);
            mapper[window + str[next_char_i]] = mapper.size();
            window = str[next_char_i];
        }
        ++next_char_i;
    }
    return encoded_sequence;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string str = readLine();
    std::vector<int> encoded_sequence = lzwEncode(str);
    std::cout << encoded_sequence.size() << "\n";
    for (const int code : encoded_sequence) {
        std::cout << code << " ";
    }
    return 0;
}
