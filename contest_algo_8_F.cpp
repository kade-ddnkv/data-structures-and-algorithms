#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Из автомата Ахо-Карасика можно взять только бор, сам автомат тут вроде не нужен, или я ошибаюсь.

const int kAlphabetSize = 26;
const int kAlphabetStartSymbol = 'a';

struct BohrVertex {
    std::vector<int> next_vertex;
    bool is_final;
};

std::vector<BohrVertex> bohr;

BohrVertex makeBohrVertex() {
    BohrVertex vertex;
    vertex.next_vertex.resize(kAlphabetSize, -1);
    vertex.is_final = false;
    return vertex;
}

void bohrInitialize() {
    bohr.push_back(makeBohrVertex());
}

void addStringToBohr(const std::string &str) {
    int bohr_i = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        int ch = str[i] - kAlphabetStartSymbol;
        if (bohr[bohr_i].next_vertex[ch] == -1) {
            bohr.push_back(makeBohrVertex());
            bohr[bohr_i].next_vertex[ch] = bohr.size() - 1;
        }
        bohr_i = bohr[bohr_i].next_vertex[ch];
    }
    bohr[bohr_i].is_final = true;
}

std::vector<std::string> readAminoAcids() {
    int number_of_amino_acids;
    std::cin >> number_of_amino_acids;
    std::vector<std::string> amino_acids(number_of_amino_acids);
    for (int i = 0; i < number_of_amino_acids; ++i) {
        std::cin >> amino_acids[i];
    }
    return amino_acids;
}

// Простой запрос.
int handleSimpleQuery(const std::string &protein, int left, int right) {
    int counter = 0;
    int bohr_i = 0;
    // Важно: "ни одна аминокислота не является началом другой".
    // Перемещаться по бору, смотря на текущий символ из белка.
    for (int i = left; i <= right; ++i) {
        int ch = protein[i] - 'a';
        if (bohr[bohr_i].next_vertex[ch] == -1) {
            return -1;
        }
        bohr_i = bohr[bohr_i].next_vertex[ch];
        if (bohr[bohr_i].is_final) {
            bohr_i = 0;
            ++counter;
        }
    }
    // Если фрагмент белка кончился, а аминокислота - еще нет.
    if (bohr_i != 0) {
        return -1;
    }
    return counter;
}

int handleComplexQuery(const std::string &protein) {
    // Инициализация и считывание переменных.
    int result_of_query = 0;
    int number_of_simple;
    int a_num, b_num, c_num, d_num, e_num;
    int l_1, r_1;
    int left, right, ans;
    std::cin >> number_of_simple >> a_num >> b_num >> c_num >> d_num >> e_num >> l_1 >> r_1;
    int protein_size = protein.size();

    // Функция для вычисления ответа простого запроса, чтобы не дублировать код.
    auto get_ans = [&]() {
        ans = handleSimpleQuery(protein, std::min(left, right) - 1, std::max(left, right) - 1);
        if (ans != -1) {
            result_of_query += ans;
        }
    };

    // Последовательное вычисление массивов l_numbers и r_numbers.
    std::vector<int> l_numbers(number_of_simple);
    std::vector<int> r_numbers(number_of_simple);
    l_numbers[0] = l_1;
    r_numbers[0] = r_1;
    left = l_numbers[0] % protein_size + 1;
    right = r_numbers[0] % protein_size + 1;
    get_ans();
    for (int i = 1; i < number_of_simple; ++i) {
        l_numbers[i] = (a_num * l_numbers[i - 1] + b_num) % e_num;
        r_numbers[i] = (c_num * r_numbers[i - 1] + d_num + ans) % e_num;

        left = l_numbers[i] % protein_size + 1;
        right = r_numbers[i] % protein_size + 1;
        get_ans();
    }
    return result_of_query % e_num;
}

int main() {
    bohrInitialize();
    std::vector<std::string> amino_acids = readAminoAcids();
    for (const std::string &amino_acid : amino_acids) {
        addStringToBohr(amino_acid);
    }
    std::string protein;
    std::cin >> protein;

    // Запуск простых запросов.
    int number_of_simple_queries;
    int left, right;
    std::cin >> number_of_simple_queries;
    for (int i = 0; i < number_of_simple_queries; ++i) {
        std::cin >> left >> right;
        std::cout << handleSimpleQuery(protein, left - 1, right - 1) << "\n";
    }

    // Запуск сложных запросов.
    int number_of_complex_queries;
    std::cin >> number_of_complex_queries;
    for (int i = 0; i < number_of_complex_queries; ++i) {
        std::cout << handleComplexQuery(protein) << "\n";
    }

    return 0;
}
