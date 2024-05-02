#include <iostream>
#include <vector>
#include <string>

class Bohr {
private:
    const int k_alphabet_size_ = 2;
    const int k_alphabet_start_symbol_ = '0';

    struct BohrVertex {
        std::vector<int> next_vertex;
        bool is_final;
    };

    std::vector<BohrVertex> bohr_;

    BohrVertex makeBohrVertex() {
        BohrVertex vertex;
        vertex.next_vertex.resize(k_alphabet_size_, -1);
        vertex.is_final = false;
        return vertex;
    }

    void bohrInitialize() {
        bohr_.push_back(makeBohrVertex());
    }

public:
    Bohr() {
        bohrInitialize();
    }

    void addStringToBohr(const std::string &str, int final_symbol) {
        int bohr_i = 0;
        for (size_t i = 0; i < str.length(); ++i) {
            int ch = str[i] - k_alphabet_start_symbol_;
            if (bohr_[bohr_i].next_vertex[ch] == -1) {
                bohr_.push_back(makeBohrVertex());
                bohr_[bohr_i].next_vertex[ch] = bohr_.size() - 1;
            }
            bohr_i = bohr_[bohr_i].next_vertex[ch];
        }
        bohr_[bohr_i].is_final = true;
        bohr_[bohr_i].next_vertex[0] = final_symbol;
    }

    char decodeSymbol(const std::string &encoded, int *start_index) const {
        int bohr_i = 0;
        int curr_symbol;
        while (!bohr_[bohr_i].is_final) {
            curr_symbol = encoded[(*start_index)++] - k_alphabet_start_symbol_;
            bohr_i = bohr_[bohr_i].next_vertex[curr_symbol];
        }
        return bohr_[bohr_i].next_vertex[0];
    }
};

Bohr getHuffmanMapperAsBohr(int unique_symbols_count) {
    Bohr bohr;
    const int k_alphabet_size = 26;
    const int k_alphabet_start_symbol = 'a';

    std::vector<std::string> mapper(k_alphabet_size);
    char curr_symbol;
    std::string curr_code;
    for (int i = 0; i < unique_symbols_count; ++i) {
        std::cin >> curr_symbol;
        std::cin >> curr_code;
        std::cin >> curr_code;
        mapper[curr_symbol - k_alphabet_start_symbol] = curr_code;
        bohr.addStringToBohr(curr_code, curr_symbol);
    }
    return bohr;
}

std::string huffmanDecode(std::string encoded, const Bohr &bohr) {
    int start_index = 0;
    char curr_symbol;
    int encoded_len = encoded.size();
    std::string decoded;
    while (start_index != encoded_len) {
        curr_symbol = bohr.decodeSymbol(encoded, &start_index);
        decoded += curr_symbol;
    }
    return decoded;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int unique_symbols_count;
    int encoded_str_len;
    std::cin >> unique_symbols_count >> encoded_str_len;
    Bohr bohr_for_decode = getHuffmanMapperAsBohr(unique_symbols_count);
    std::string encoded;
    std::cin >> encoded;
    std::cout << huffmanDecode(encoded, bohr_for_decode);
    return 0;
}
