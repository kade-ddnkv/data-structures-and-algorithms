#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

using std::string;

std::string genRandom(const int len) {
    static const char kAlphabet[] =
        "_0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += kAlphabet[rand() % (sizeof(kAlphabet) - 1)];
    }
    return tmp_s;
}

struct Hasher {
    const int p = 31;
    size_t operator()(const string& s) const {
        size_t hash = 0, p_pow = 1;
        for (size_t i = 0; i < s.size(); ++i) {
            hash += (s[i] - 'a' + 1) * p_pow;
            p_pow *= p;
        }
        return hash;
    }
};

string operator*(string a, unsigned int b) {
    string output;
    while (b--) {
        output += a;
    }
    return output;
}

void generate(std::ofstream& file) {
    // Я понимаю, что столько вложенных циклов - это плохо, но лучше решения я не придумал.
    std::string block_aa = "Aa";

    // Циклы, которые выводят небольшое количество значений, отключены.

    // Цикл для значений типа aAaAaA.
    //    file << static_cast<char>('a' + offset) << "\n";
    //    for (int i = 1; i <= 14; ++i) {
    //        file << static_cast<char>('a' + offset) + (block_aa * i) << "\n";
    //    }

    // Цикл для значений типа BBBBBBaABaAaA
    // Блок "B...Ba" в начале.
    for (int i = 0; i <= 28; ++i) {
        std::string curr = "B" + std::string(i, 'B') + "a";
        file << curr << "\n";

        // Выбор количества блоков типа AB...Ba.
        for (int j = 4; j <= (30 - i - 2) / 2; ++j) {

            // Добавление B внутрь первого блока Aa.
            // Верхнее значение 7 взято с потолка.
            for (int k = 0; k < 7; ++k) {
                std::string first_block_change = curr;
                first_block_change += 'A' + std::string(k, 'B') + 'a';

                // Добавление B внутрь второго блока Aa.
                for (int l = 0; l < 7; ++l) {
                    std::string second_block_change = first_block_change;
                    second_block_change += 'A' + std::string(l, 'B') + 'a';

                    // Добавление B внутрь третьего блока Aa.
                    for (int m = 0; m < 7; ++m) {
                        std::string third_block_change = second_block_change;
                        third_block_change += 'A' + std::string(m, 'B') + 'a';

                        // Добавление B внутрь четвертого блока Aa.
                        for (int n = 0; n < 7; ++n) {
                            std::string forth_block_change = third_block_change;
                            forth_block_change += 'A' + std::string(n, 'B') + 'a';

                            forth_block_change += block_aa * (j - 4);

                            if (forth_block_change.size() <= 30) {
                                file << forth_block_change << "\n";
                            }
                        }
                    }
                }
            }
        }
    }

    // Это цикл для значений типа aAaAaABBBa. (блоки aA в начале)
    //    for (int i = 0; i <= 13; ++i) {
    //        std::string curr = static_cast<char>('a' + offset) + std::string(1, 'A');
    //        curr += aA * i;
    //        for (int j = 1; j <= 30 - (i*2 + 2) - 1; ++j) {
    //            std::string curr_with_bs = curr;
    //            curr_with_bs += B * j;
    //            file << curr_with_bs + 'a' << "\n";
    //            //if (curr_with_bs.size() + 1 > 30) {
    //            //    throw new std::out_of_range("");
    //            //}
    //        }
    //    }
}

void pick9000(std::ifstream& in) {
    std::ofstream out("crackme9000");
    std::vector<std::string> lines(9000);
    Hasher h;
    std::string curr;
    for (int i = 0; i < 9000;) {
        in >> curr;
        // Отбираются только большие строки.
        if (curr.size() == 28) {
            lines[i] = curr;
            ++i;
        }
    }
    // Shuffle добавляет ко времени исполнения около 0.6 секунд.
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(lines.begin(), lines.end(), g);
    for (int i = 0; i < 9000; ++i) {
        out << lines[i] << "\n";
    }
    out.close();
}

int main() {
    std::ofstream out("crackme");
    generate(out);
    out.close();

    std::ifstream in("crackme");
    pick9000(in);
    in.close();

    return 0;
}