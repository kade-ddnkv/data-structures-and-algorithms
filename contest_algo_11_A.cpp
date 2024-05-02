#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

std::string getLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int getMinNumberOfRiverCrossing(std::string river) {
    std::vector<std::vector<bool>> has_cross(2);
    has_cross[0].resize(river.length());
    has_cross[1].resize(river.length());
    for (size_t i = 0; i < river.length(); ++i) {
        has_cross[0][i] = river[i] == 'L' || river[i] == 'B';
        has_cross[1][i] = river[i] == 'R' || river[i] == 'B';
    }

    std::vector<std::vector<int>> dynamic_crossings(2);
    dynamic_crossings[0].resize(river.length() + 1);
    dynamic_crossings[1].resize(river.length() + 1);
    dynamic_crossings[0][river.length()] = 1;
    dynamic_crossings[1][river.length()] = 0;

    for (int x = static_cast<int>(river.length()) - 1; x >= 0; --x) {
        dynamic_crossings[0][x] = std::min(has_cross[0][x] + dynamic_crossings[0][x + 1],
                                           has_cross[1][x] + dynamic_crossings[1][x + 1] + 1);
        dynamic_crossings[1][x] = std::min(has_cross[1][x] + dynamic_crossings[1][x + 1],
                                           has_cross[0][x] + dynamic_crossings[0][x + 1] + 1);
    }
    return dynamic_crossings[0][0];
}

int main() {
    std::string river = getLine();
    std::cout << getMinNumberOfRiverCrossing(river);
    return 0;
}
