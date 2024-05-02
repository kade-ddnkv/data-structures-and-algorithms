#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

std::vector<int> getPrefixArray(std::string str) {
    std::vector<int> prefix_array(str.size());
    prefix_array[0] = 0;
    int p_rec_res;
    int str_size = str.size();
    for (int i = 0; i < str_size - 1; ++i) {
        p_rec_res = prefix_array[i];
        while (p_rec_res > 0 && str[i + 1] != str[p_rec_res]) {
            p_rec_res = prefix_array[p_rec_res - 1];
        }
        if (str[i + 1] == str[p_rec_res]) {
            ++p_rec_res;
        }
        prefix_array[i + 1] = p_rec_res;
    }
    return prefix_array;
}

int getNumberOfUniqueChars(std::string str) {
    std::vector<bool> presented_chars(26);
    for (const char &ch : str) {
        presented_chars[ch] = true;
    }
    int number_of_unique = 0;
    for (const bool &elem : presented_chars) {
        number_of_unique += elem;
    }
    return number_of_unique;
}

int getNumberOfGoodClicks(int word_length, int buttons_count, int max_bad_clicks,
                          std::string word) {
    // Преобразую слово.
    for (int i = 0; i < word_length; ++i) {
        word[i] -= 'a';
    }
    int number_of_unique = getNumberOfUniqueChars(word);
    // Если есть буква, которой нет в слове Пети, решение очевидно.
    if (buttons_count > number_of_unique) {
        return word_length - 1 + max_bad_clicks * (word_length - 1) + 1;
    }

    // В процессе работы оказалось, что автомат КМП здесь не обязателен.
    // Для каждого места нужно найти максимальный сдвиг, который может сделать Ваня, поставив
    // неправильную букву.

    std::vector<std::pair<int, int>> shifts_lengths;
    int p_rec_res;
    std::vector<int> prefix_array = getPrefixArray(word);
    for (int letter_place = 1; letter_place < word_length; ++letter_place) {
        for (int alphabet_letter = 0; alphabet_letter < number_of_unique; ++alphabet_letter) {
            if (alphabet_letter == word[letter_place]) {
                continue;
            }
            // p_rec_res - это место, на которое встанет указатель после встревания Вани.
            p_rec_res = prefix_array[letter_place - 1];
            while (p_rec_res > 0 && word[p_rec_res] != alphabet_letter) {
                p_rec_res = prefix_array[p_rec_res - 1];
            }
            if (alphabet_letter == word[p_rec_res]) {
                ++p_rec_res;
            }
            // letter_place - это место текущей буквы в слове.
            // То есть letter_place - p_rec_res = длина сдвига
            shifts_lengths.push_back(std::make_pair(letter_place, letter_place - p_rec_res));
        }
    }

    // Поиск максимального сдвига.
    int max_shift_index;
    int max_shift = -1;
    for (size_t i = 0; i < shifts_lengths.size(); ++i) {
        if (shifts_lengths[i].second > max_shift) {
            max_shift = shifts_lengths[i].second;
            max_shift_index = shifts_lengths[i].first;
        }
    }
    if (max_shift == -1) {
        return word_length;
    }

    // Результат = кол-во букв до встревания Вани
    // + кол-во встреваний * кол-во букв от конца встревания до начала встревания
    // + кол-во букв после встревания
    return (max_shift_index) + (max_shift * max_bad_clicks) + (word_length - max_shift_index);
}

int main() {
    int word_length, buttons_count, max_bad_clicks;
    std::cin >> word_length >> buttons_count >> max_bad_clicks;
    std::string word;
    std::cin >> word;
    std::cout << getNumberOfGoodClicks(word_length, buttons_count, max_bad_clicks, word);
    return 0;
}
