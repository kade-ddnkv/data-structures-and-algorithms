#include <iostream>
#include <vector>
#include <string>

// Задача: найти в автомате Ахо-Корасика цикл
// (не включающий терминальные вершины или вершины с терминальными ссылками).

const int kAlphabetSize = 2;
const int kAlphabetStartSymbol = '0';

// Цвета для прохода в глубину и поиска цикла.
// Белый цвет - в вершину не заходили.
// Серый цвет - в вершину уже зашли на текущем проходе -> цикл найден.
// Черный цвет - из вершины вышли, не найдя цикла.
enum VertexColors { BLACK, GREY, WHITE };

struct BohrVertex {
    std::vector<int> next_vertex;
    bool is_final;
    int suffix_link;
    std::vector<int> move;
    int parent;
    int symbol;
    int terminal_link;

    VertexColors color;
};

std::vector<BohrVertex> bohr;

BohrVertex makeBohrVertex(int parent, int symbol) {
    BohrVertex vertex;

    vertex.next_vertex.resize(kAlphabetSize, -1);
    vertex.move.resize(kAlphabetSize, -1);

    vertex.is_final = false;
    vertex.suffix_link = -1;
    vertex.parent = parent;
    vertex.symbol = symbol;
    vertex.terminal_link = -1;
    vertex.color = WHITE;
    return vertex;
}

void bohrInitialize() {
    bohr.push_back(makeBohrVertex(0, 'a'));
}

void addStringToBohr(const std::string& str) {
    int bohr_i = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        int ch = str[i] - kAlphabetStartSymbol;
        if (bohr[bohr_i].next_vertex[ch] == -1) {
            bohr.push_back(makeBohrVertex(bohr_i, ch));
            bohr[bohr_i].next_vertex[ch] = bohr.size() - 1;
        }
        bohr_i = bohr[bohr_i].next_vertex[ch];
    }
    bohr[bohr_i].is_final = true;
}

int getMove(int vertex, int ch);

int getSuffix(int vertex) {
    if (bohr[vertex].suffix_link == -1) {
        if (vertex == 0 || bohr[vertex].parent == 0) {
            bohr[vertex].suffix_link = 0;
        } else {
            bohr[vertex].suffix_link = getMove(getSuffix(bohr[vertex].parent), bohr[vertex].symbol);
        }
    }
    return bohr[vertex].suffix_link;
}

int getMove(int vertex, int ch) {
    if (bohr[vertex].move[ch] == -1) {
        if (bohr[vertex].next_vertex[ch] != -1) {
            bohr[vertex].move[ch] = bohr[vertex].next_vertex[ch];
        } else if (vertex == 0) {
            bohr[vertex].move[ch] = 0;
        } else {
            bohr[vertex].move[ch] = getMove(getSuffix(vertex), ch);
        }
    }
    return bohr[vertex].move[ch];
}

int getTerminalLink(int vertex) {
    if (bohr[vertex].terminal_link == -1) {
        int suffix = getSuffix(vertex);
        if (suffix == 0) {
            bohr[vertex].terminal_link = 0;
        } else {
            // Рекурсивные вызовы до первой терминальной вершины.
            bohr[vertex].terminal_link = (bohr[suffix].is_final) ? suffix : getTerminalLink(suffix);
        }
    }
    return bohr[vertex].terminal_link;
}

std::vector<std::string> readViruses() {
    int number_of_viruses;
    std::cin >> number_of_viruses;
    std::vector<std::string> viruses(number_of_viruses);
    for (int i = 0; i < number_of_viruses; ++i) {
        std::cin >> viruses[i];
    }
    return viruses;
}

bool findLoopInAutomaton(int vertex) {
    // Не заходить в терминальные вершины.
    if (bohr[vertex].is_final) {
        return false;
    }
    // Сигнал о найденном цикле - на проходе в глубину найдена серая вершина.
    if (bohr[vertex].color == GREY) {
        return true;
    }
    // Не заходить в вершины с терминальными ссылками.
    if (getTerminalLink(vertex) != 0) {
        return false;
    }
    if (bohr[vertex].color == WHITE) {
        bohr[vertex].color = GREY;
        for (int i = 0; i < kAlphabetSize; ++i) {
            if (findLoopInAutomaton(getMove(vertex, i))) {
                return true;
            }
        }
        bohr[vertex].color = BLACK;
        return false;
    }
    return false;
}

int main() {
    bohrInitialize();
    std::vector<std::string> viruses = readViruses();
    for (const std::string& virus : viruses) {
        addStringToBohr(virus);
    }
    if (findLoopInAutomaton(0)) {
        std::cout << "TAK";
    } else {
        std::cout << "NIE";
    }
    return 0;
}
