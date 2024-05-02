#include <iostream>
#include <vector>
#include <algorithm>

void readData(int *height, int *width, std::vector<std::vector<char>> *chars) {
    std::cin >> *height >> *width;
    chars->resize(*height + 1);
    (*chars)[0].resize(*width + 2, '#');
    for (int i = 1; i <= *height; ++i) {
        (*chars)[i].resize(*width + 2);
        (*chars)[i][0] = '#';
        for (int j = 1; j <= *width; ++j) {
            std::cin >> (*chars)[i][j];
        }
        (*chars)[i][*width + 1] = '#';
    }
}

int getAllPostersVariants(int height, int width, const std::vector<std::vector<char>> &chars) {
    std::vector<std::vector<int>> posters;
    posters.resize(height + 1);
    for (int i = 0; i <= height; ++i) {
        posters[i].resize(width + 2);
    }

    for (int y = 1; y <= height; ++y) {
        for (int x = 1; x <= width; ++x) {
            if (chars[y][x] == chars[y - 1][x - 1] && chars[y][x] == chars[y - 1][x] &&
                chars[y][x] == chars[y - 1][x + 1] && chars[y][x] == chars[y - 2][x]) {
                posters[y][x] =
                    1 + std::min({posters[y - 1][x - 1], posters[y - 2][x], posters[y - 1][x + 1]});
            } else {
                posters[y][x] = 1;
            }
        }
    }

    int sum = 0;
    for (int y = 1; y <= height; ++y) {
        for (int x = 1; x <= width; ++x) {
            sum += posters[y][x];
        }
    }
    return sum;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int height, width;
    std::vector<std::vector<char>> chars;
    readData(&height, &width, &chars);
    std::cout << getAllPostersVariants(height, width, chars);
    return 0;
}
