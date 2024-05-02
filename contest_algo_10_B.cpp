#include <iostream>
#include <unordered_map>
#include <string>

// Скорее всего, по-хорошему это нужно делать с какой-то модификацией автомата КМП.

std::unordered_map<int, int> findFirstMatchNaive(const std::string& encoded_document,
                                                 const std::string& original_part) {
    std::unordered_map<int, int> map_decode;
    std::unordered_map<int, int> map_encode;

    // "Прикладывание" куска к каждому символу в строке (O(n^2)).
    for (size_t doc_i = 0; doc_i <= encoded_document.length() - original_part.length(); ++doc_i) {
        map_decode.clear();
        map_encode.clear();
        size_t part_j;
        // Само "прикладывание".
        for (part_j = 0; part_j < original_part.length(); ++part_j) {
            if (map_decode.find(encoded_document[doc_i + part_j]) == map_decode.end()) {
                // Если символ полностью новый.
                if (map_encode.find(original_part[part_j]) == map_encode.end()) {
                    map_decode[encoded_document[doc_i + part_j]] = original_part[part_j];
                    map_encode[original_part[part_j]] = encoded_document[doc_i + part_j];
                } else {
                    break;
                }
            } else {
                if (map_encode.find(original_part[part_j]) == map_encode.end()) {
                    break;
                } else if (map_decode[encoded_document[doc_i + part_j]] != original_part[part_j] ||
                           encoded_document[doc_i + part_j] != map_encode[original_part[part_j]]) {
                    // Если не совпадают значения декодера и кодера для следующего символа.
                    break;
                }
            }
        }
        // Если кусок был полностью обработан.
        if (part_j == original_part.length()) {
            return map_decode;
        }
    }
    throw std::exception();
}

int main() {
    std::string encoded_document, original_part;
    std::cin >> encoded_document >> original_part;
    try {
        auto map_decode = findFirstMatchNaive(encoded_document, original_part);
        std::cout << "Possible\n";
        for (size_t i = 0; i < encoded_document.length(); ++i) {
            if (map_decode.find(encoded_document[i]) != map_decode.end()) {
                std::cout << char(map_decode[encoded_document[i]]);
            } else {
                std::cout << "?";
            }
        }
    } catch (...) {
        std::cout << "Impossible";
    }
    return 0;
}
