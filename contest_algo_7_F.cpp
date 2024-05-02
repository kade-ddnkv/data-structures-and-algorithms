#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

int minFromThree(const int &num1, const int &num2, const int &num3) {
    return std::min(std::min(num1, num2), num3);
}

bool isSimilarInLevenshteinDistance(const std::string &first, const std::string &second,
                                    int max_acceptable_mistakes_number) {
    std::vector<std::vector<int>> matrix(first.size() + 1);
    for (size_t i = 0; i < first.size() + 1; ++i) {
        matrix[i].resize(second.size() + 1);
    }
    for (size_t i = 0; i < first.size() + 1; ++i) {
        matrix[i][0] = i;
    }
    for (size_t i = 0; i < second.size() + 1; ++i) {
        matrix[0][i] = i;
    }
    for (size_t i = 1; i < first.size() + 1; ++i) {
        for (size_t j = 1; j < second.size() + 1; ++j) {
            matrix[i][j] =
                minFromThree(matrix[i - 1][j] + 1, matrix[i][j - 1] + 1,
                             matrix[i - 1][j - 1] + (first[i - 1] == second[j - 1] ? 0 : 1));
        }
        if (i > 0) {
            matrix[i - 1].clear();
        }
    }
    return matrix[first.size()][second.size()] <= max_acceptable_mistakes_number;
}

std::vector<std::string> readCommandsVector() {
    int number_of_commands;
    std::cin >> number_of_commands;
    std::vector<std::string> commands(number_of_commands);
    for (int i = 0; i < number_of_commands; ++i) {
        std::cin >> commands[i];
    }
    return commands;
}

std::vector<std::string> alphabet = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
                                     "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
                                     "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
                                     "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
                                     "w", "x", "y", "z", "+", "-", "_", "|", "=", "/"};

void generatePossibleMistakes(std::string query, std::unordered_set<std::string> *possible,
                              int curr_acceptable_mistakes_number) {
    if (curr_acceptable_mistakes_number == 0) {
        possible->insert(query);
        return;
    }
    std::string generated_mistake;
    // Удаление.
    for (size_t i = 0; i < query.size(); ++i) {
        generated_mistake = query;
        generated_mistake.erase(i, 1);
        //        possible->push_back(generated_mistake);
        generatePossibleMistakes(generated_mistake, possible, curr_acceptable_mistakes_number - 1);
    }
    // Вставка.
    for (size_t i = 0; i < query.size(); ++i) {
        for (std::string letter : alphabet) {
            generated_mistake = query;
            generated_mistake.insert(i, letter);
            //            possible->push_back(generated_mistake);
            generatePossibleMistakes(generated_mistake, possible,
                                     curr_acceptable_mistakes_number - 1);
        }
    }
    // Замена.
    for (size_t i = 0; i < query.size(); ++i) {
        for (std::string letter : alphabet) {
            generated_mistake = query;
            generated_mistake.replace(i, 1, letter);
            //            possible->push_back(generated_mistake);
            generatePossibleMistakes(generated_mistake, possible,
                                     curr_acceptable_mistakes_number - 1);
        }
    }
}

int getNumberOfSimilarCommands(std::string query, const std::vector<std::string> &valid_commands,
                               int max_acceptable_mistakes_number) {
    int number_of_similar = 0;
    std::unordered_set<std::string> possible;
    generatePossibleMistakes(query, &possible, max_acceptable_mistakes_number);
    //    for (const std::string &command : valid_commands) {
    //        if (isSimilarInLevenshteinDistance(query, command, max_acceptable_mistakes_number)) {
    //            ++number_of_similar;
    //        }
    //    }
    for (const std::string &mistake : possible) {
        if (std::find(valid_commands.begin(), valid_commands.end(), mistake) !=
            valid_commands.end()) {
            ++number_of_similar;
        }
    }
    return number_of_similar;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::vector<std::string> valid_commands = readCommandsVector();
    int max_acceptable_mistakes_number;
    std::cin >> max_acceptable_mistakes_number;
    std::vector<std::string> queries = readCommandsVector();
    for (const std::string &query : queries) {
        std::cout << getNumberOfSimilarCommands(query, valid_commands,
                                                max_acceptable_mistakes_number)
                  << "\n";
    }
    return 0;
}
