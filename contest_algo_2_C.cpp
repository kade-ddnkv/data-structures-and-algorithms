#include <iostream>
#include <vector>

bool tryCutWiresIntoPieces(const std::vector<int> &wires, int number_of_pieces, int piece_length) {
    int pieces_count = 0;
    for (size_t i = 0; i < wires.size(); ++i) {
        pieces_count += wires[i] / piece_length;
        if (pieces_count >= number_of_pieces) {
            return true;
        }
    }
    return false;
}

int findMaxPieceLength(const std::vector<int> &wires, int number_of_pieces) {
    int left = 0;
    int right;
    int64_t sum_all_wires = 0;
    for (size_t i = 0; i < wires.size(); ++i) {
        sum_all_wires += wires[i];
    }
    right = sum_all_wires / number_of_pieces + 1;

    int mid;
    while (right - left > 1) {
        mid = left + (right - left) / 2;
        if (tryCutWiresIntoPieces(wires, number_of_pieces, mid)) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return right - 1;
}

int main() {
    int number_of_wires;
    int number_of_pieces;
    std::cin >> number_of_wires >> number_of_pieces;

    std::vector<int> wires(number_of_wires);
    for (int i = 0; i < number_of_wires; ++i) {
        std::cin >> wires[i];
    }

    std::cout << findMaxPieceLength(wires, number_of_pieces);
    return 0;
}
