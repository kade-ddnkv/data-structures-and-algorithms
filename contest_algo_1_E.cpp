#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

int main() {
    int participant_teams_number;
    int final_teams_counter;
    int from_one_university_number;
    std::string task_numbers;
    std::stringstream task_numbers_stream;
    getline(std::cin, task_numbers);
    task_numbers_stream << task_numbers;
    task_numbers_stream >> participant_teams_number >> final_teams_counter >>
        from_one_university_number;

    std::vector<std::string> team_names(participant_teams_number);
    std::vector<int> team_numbers(participant_teams_number);

    for (int i = 0; i < participant_teams_number; ++i) {
        getline(std::cin, team_names[i]);
    }
    for (int i = 0; i < participant_teams_number; ++i) {
        std::cin >> team_numbers[i];
    }
    std::unordered_map<std::string, int> inside_university_counter(team_names.size());
    for (size_t i = 0; i < team_names.size(); ++i) {
        inside_university_counter[team_names[i]] = from_one_university_number;
    }

    size_t curr_team_index = 0;
    while (final_teams_counter != 0 && curr_team_index < team_names.size()) {
        if (inside_university_counter[team_names[curr_team_index]] != 0) {
            std::cout << team_names[curr_team_index] << " #" << team_numbers[curr_team_index]
                      << "\n";
            --inside_university_counter[team_names[curr_team_index]];
            --final_teams_counter;
        }
        ++curr_team_index;
    }

    return 0;
}
