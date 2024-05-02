#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> readAttemptsFromConsole() {
    int number_of_attempts;
    std::cin >> number_of_attempts;
    std::unordered_map<std::string, int> attempts;
    int curr_team;
    std::string curr_word;
    for (int i = 0; i < number_of_attempts; ++i) {
        std::cin >> curr_team >> curr_word;
        attempts.insert_or_assign(curr_word, curr_team);
    }
    return attempts;
}

int main() {
    int number_of_teams;
    std::cin >> number_of_teams;
    std::vector<int> teams(number_of_teams);

    auto attempts = readAttemptsFromConsole();

    for (auto& attempt : attempts) {
        ++teams[attempt.second - 1];
    }

    for (auto& team : teams) {
        std::cout << team << " ";
    }

    return 0;
}
