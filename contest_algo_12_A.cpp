#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

std::vector<std::vector<std::vector<int64_t>>> rgb;
std::vector<int> red;
std::vector<int> green;
std::vector<int> blue;

void readData() {
    int n_red, n_green, n_blue;
    std::cin >> n_red >> n_green >> n_blue;
    red.resize(n_red);
    green.resize(n_green);
    blue.resize(n_blue);
    for (int i = 0; i < n_red; ++i) {
        std::cin >> red[i];
    }
    for (int i = 0; i < n_green; ++i) {
        std::cin >> green[i];
    }
    for (int i = 0; i < n_blue; ++i) {
        std::cin >> blue[i];
    }
}

int64_t computeSubtask(int red_c, int green_c, int blue_c) {
    if (rgb[red_c][green_c][blue_c] != -1) {
        return rgb[red_c][green_c][blue_c];
    }
    rgb[red_c][green_c][blue_c] = 0;
    if (red_c == 0) {
        int min_c = std::min(green_c, blue_c);
        for (int i = 0; i < min_c; ++i) {
            rgb[red_c][green_c][blue_c] += 1LL * green[green_c - i - 1] * blue[blue_c - i - 1];
        }
        return rgb[red_c][green_c][blue_c];
    }
    if (green_c == 0) {
        int min_c = std::min(red_c, blue_c);
        for (int i = 0; i < min_c; ++i) {
            rgb[red_c][green_c][blue_c] += 1LL * red[red_c - i - 1] * blue[blue_c - i - 1];
        }
        return rgb[red_c][green_c][blue_c];
    }
    if (blue_c == 0) {
        int min_c = std::min(red_c, green_c);
        for (int i = 0; i < min_c; ++i) {
            rgb[red_c][green_c][blue_c] += 1LL * red[red_c - i - 1] * green[green_c - i - 1];
        }
        return rgb[red_c][green_c][blue_c];
    }

    rgb[red_c][green_c][blue_c] = std::max(
        {1LL * red[red_c - 1] * green[green_c - 1] + computeSubtask(red_c - 1, green_c - 1, blue_c),
         1LL * blue[blue_c - 1] * green[green_c - 1] +
             computeSubtask(red_c, green_c - 1, blue_c - 1),
         1LL * blue[blue_c - 1] * red[red_c - 1] + computeSubtask(red_c - 1, green_c, blue_c - 1)});

    return rgb[red_c][green_c][blue_c];
}

int64_t getMaxSumSquare() {
    rgb.resize(red.size() + 1);
    for (size_t i = 0; i < red.size() + 1; ++i) {
        rgb[i].resize(green.size() + 1);
        for (size_t j = 0; j < green.size() + 1; ++j) {
            rgb[i][j].resize(blue.size() + 1, -1);
        }
    }

    std::sort(red.begin(), red.end());
    std::sort(green.begin(), green.end());
    std::sort(blue.begin(), blue.end());

    int64_t result = computeSubtask(red.size(), green.size(), blue.size());
    for (size_t i = 0; i < red.size() + 1; ++i) {
        std::cout << "red=" << i << "\n";
        for (size_t j = 0; j < green.size() + 1; ++j) {
            for (size_t k = 0; k < blue.size() + 1; ++k) {
                std::cout << rgb[i][j][k] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    return result;
}

int main() {
    readData();
    std::cout << getMaxSumSquare();
    return 0;
}

// int main() {
//     std::ofstream out("temp_out");
//     out << "100 100 100\n";
////     srand(time(nullptr));
//     for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 100; ++j) {
//             out << 10000000 << " ";
//         }
//         out << "\n";
//     }
//     return 0;
// }
