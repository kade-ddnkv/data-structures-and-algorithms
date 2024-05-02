#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

std::vector<int> readEncodedFragments(int number_of_fragments) {
    std::vector<int> fragments(number_of_fragments);
    for (int i = 0; i < number_of_fragments; ++i) {
        std::cin >> fragments[i];
    }
    return fragments;
}

const int kAlphabetSize = 128;

void initLzwMapper(std::unordered_map<int, std::string> *mapper) {
    for (int i = 0; i < kAlphabetSize; ++i) {
        (*mapper)[i] = std::string(1, i);
    }
}

std::string lzwDecode(const std::vector<int> &fragments) {
    std::string result_str;
    std::string prev_mapped_fragment;
    std::unordered_map<int, std::string> mapper;

    initLzwMapper(&mapper);
    result_str += mapper[fragments[0]];
    prev_mapped_fragment = mapper[fragments[0]];
    for (size_t i = 1; i < fragments.size(); ++i) {
        if (mapper.find(fragments[i]) != mapper.end()) {
            result_str += mapper[fragments[i]];
            mapper[mapper.size()] = prev_mapped_fragment + mapper[fragments[i]][0];
        } else {
            // Иначе это повторяющаяся буква.
            result_str += prev_mapped_fragment + prev_mapped_fragment[0];
            mapper[mapper.size()] = prev_mapped_fragment + prev_mapped_fragment[0];
        }
        prev_mapped_fragment = mapper[fragments[i]];
    }
    return result_str;
}

int main() {
    int number_of_fragments;
    std::cin >> number_of_fragments;
    std::vector<int> encoded_fragments = readEncodedFragments(number_of_fragments);
    std::cout << lzwDecode(encoded_fragments);
    return 0;
}
