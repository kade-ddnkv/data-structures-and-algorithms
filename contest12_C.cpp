#include <iostream>
#include <queue>
#include <tuple>

std::tuple<std::queue<int>, std::queue<int>> readCardDecksFromConsole() {
    std::queue<int> first;
    std::queue<int> second;

    for (int i = 0; i < 5; ++i) {
        int curr;
        std::cin >> curr;
        first.push(curr);
    }

    for (int i = 0; i < 5; ++i) {
        int curr;
        std::cin >> curr;
        second.push(curr);
    }
    return {first, second};
}

void startDrunkGame(std::queue<int> first_deck, std::queue<int> second_deck) {
    int turn_count = 0;

    while (!first_deck.empty() && !second_deck.empty() && turn_count != 1000000) {
        if ((first_deck.front() > second_deck.front() &&
             !(first_deck.front() == 9 && second_deck.front() == 0)) ||
            (first_deck.front() == 0 && second_deck.front() == 9)) {
            first_deck.push(first_deck.front());
            first_deck.push(second_deck.front());
            first_deck.pop();
            second_deck.pop();
        } else {
            second_deck.push(first_deck.front());
            second_deck.push(second_deck.front());
            first_deck.pop();
            second_deck.pop();
        }
        ++turn_count;
    }

    if (turn_count == 1000000) {
        std::cout << "botva";
    } else {
        std::cout << (first_deck.empty() ? "second" : "first") << " " << turn_count;
    }
}

int main() {
    auto[first_card_deck, second_card_deck] = readCardDecksFromConsole();

    startDrunkGame(first_card_deck, second_card_deck);

    return 0;
}
