#include <iostream>
#include <sstream>
#include <list>
#include <string>

std::list<int> readBallsListFromConsole() {
    std::list<int> balls;
    std::string input_line;
    getline(std::cin, input_line);
    std::stringstream in(input_line);

    int curr_ball;
    while (in >> curr_ball) {
        balls.push_back(curr_ball);
    }
    return balls;
}

// Returns number of destroyed balls;
int destroyChainsOfBalls(std::list<int> *balls) {
    int all_erased = 0;
    int curr_erased = -1;
    int repeats;
    int prev_ball;

    while (curr_erased != 0) {
        repeats = 0;
        prev_ball = -1;
        auto it = balls->begin();
        auto begin = balls->begin();
        auto end = balls->end();

        for (; it != balls->end(); ++it) {
            if (*it == prev_ball) {
                ++repeats;
            } else if (repeats >= 3) {
                end = it;
                break;
            } else {
                begin = it;
                repeats = 1;
            }
            prev_ball = *it;
        }
        if (repeats >= 3) {
            balls->erase(begin, end);
            curr_erased = repeats;
            all_erased += curr_erased;
        } else {
            curr_erased = 0;
        }
    }

    return all_erased;
}

int main() {
    std::list<int> balls = readBallsListFromConsole();
    std::cout << destroyChainsOfBalls(&balls);

    return 0;
}
