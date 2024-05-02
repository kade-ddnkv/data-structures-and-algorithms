#include <iostream>
#include <vector>

std::vector<std::vector<int64_t>> board;

void readData(int *height, int *width) {
    std::cin >> *height >> *width;
}

void fillBoard(int height, int width) {
    board.resize(height + 3);
    for (int i = 0; i < height + 3; ++i) {
        board[i].resize(width + 3);
    }

    board[2][2] = 1;
    for (int y = 3; y <= height + 1; ++y) {
        for (int x = 2; x <= y && x <= width + 1; ++x) {
            int curr_y = y - x + 2;
            board[curr_y][x] = board[curr_y + 1][x - 2] + board[curr_y - 1][x - 2] +
                               board[curr_y - 2][x - 1] + board[curr_y - 2][x + 1];
        }
    }
    int last_y = height + 1;
    for (int i = 0; i < width - 1; ++i) {
        for (int x = 3 + i; x - 3 - i < height && x <= width + 1; ++x) {
            int curr_y = last_y - x + 3 + i;
            board[curr_y][x] = board[curr_y + 1][x - 2] + board[curr_y - 1][x - 2] +
                               board[curr_y - 2][x - 1] + board[curr_y - 2][x + 1];
        }
    }
}

int main() {
    int height, width;
    readData(&height, &width);
    fillBoard(height, width);
    std::cout << board[height + 1][width + 1];
    return 0;
}
