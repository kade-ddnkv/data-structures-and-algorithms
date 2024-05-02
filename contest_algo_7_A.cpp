#include <iostream>
#include <string>
#include <vector>

std::string readLine() {
    std::string line;
    getline(std::cin, line);
    return line;
}

std::vector<int> getEdgesArray(std::string str) {
    std::vector<int> br(str.size());
    if (str.empty()) {
        return br;
    }
    br[0] = 0;
    int br_recursive_result;
    int str_size = str.size();
    for (int i = 0; i < str_size - 1; ++i) {
        if (str[i + 1] == str[br[i]]) {
            br[i + 1] = br[i] + 1;
        } else {
            br_recursive_result = br[i];
            while (br_recursive_result != 0 && str[i + 1] != str[br_recursive_result]) {
                br_recursive_result = br[br_recursive_result - 1];
            }
            br[i + 1] = br_recursive_result;
        }
    }
    return br;
}

std::vector<int> knuthMorrisPrattAlgorithm(std::string substr, std::string text) {
    std::vector<int> br = getEdgesArray(substr);
    std::vector<int> matches;
    int sub_i = 0;
    int substr_size = substr.size();
    int text_size = text.size();
    for (int i = 0; i < text_size; ++i) {
        while (sub_i > 0 && substr[sub_i] != text[i]) {
            sub_i = br[sub_i - 1];
        }
        if (substr[sub_i] == text[i]) {
            ++sub_i;
        }
        if (sub_i == substr_size) {
            matches.push_back(i - substr_size + 1);
            sub_i = br[substr_size - 1];
        }
    }
    return matches;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::string substr = readLine();
    std::string text = readLine();
    std::vector<int> matches = knuthMorrisPrattAlgorithm(substr, text);
    std::cout << matches.size() << "\n";
    for (const int &match : matches) {
        std::cout << match << "\n";
    }
    return 0;
}
