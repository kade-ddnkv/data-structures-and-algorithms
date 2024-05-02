#include <iostream>
#include <unordered_set>
#include <string>
#include <ctime>
#include <cctype>
#include <random>
#include <cstdlib>

using std::string;

bool checkUserName(const std::string& user) {
    for (char c : user) {
        if (!isalpha(c) && !isdigit(c) && c != '_') {
            return false;
        }
    }
    return true;
}

bool getLine(std::string* line, size_t max_length) {
    line->clear();
    while (std::cin && line->size() <= max_length) {
        int symbol = std::cin.get();
        if (symbol == '\n') {
            return true;
        }
        line->push_back(symbol);
    }
    return false;
}

std::vector<std::string> getUsers(size_t max_count) {
    std::vector<std::string> users;
    std::string cur_line;
    while (users.size() < max_count) {
        bool is_valid = getLine(&cur_line, 30u);
        if (std::cin.eof()) {
            break;
        }
        if (!is_valid) {
            std::cout << "Too long username " << cur_line << '\n';
            exit(0);
        }
        if (checkUserName(cur_line)) {
            users.push_back(cur_line);
        } else {
            std::cout << "Incorrect username " << cur_line << '\n';
            exit(0);
        }
    }
    return users;
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

int main2() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::mt19937 gen(time(nullptr));
    std::uniform_int_distribution<int> dist(30'000, 1'000'000);
    size_t init_size = dist(gen);
    Hasher hasher;
    std::unordered_set<string, Hasher> users(init_size, hasher);

    auto users_list = getUsers(9'000u);
    auto start_time = std::clock();

    for (const auto& user : users_list) {
        users.insert(user);
    }

    auto end_time = std::clock();
    double spent = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    std::cout << "Spent " << spent << " seconds\n";
    if (spent > 0.75) {
        std::cout << "Shit happens\n";
        return 1;
    }

    return 0;
}

#include <fstream>

using std::string;

std::string genRandom(const int len) {
    static const char kAlphanum[] =
        "_0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += kAlphanum[rand() % (sizeof(kAlphanum) - 1)];
    }

    return tmp_s;
}

string operator * (string a, unsigned int b) {
    string output;
    while (b--) {
        output += a;
    }
    return output;
}

void write(std::ofstream &file, int offset) {
    //char first_char = 'A';
    std::string Aa = "Aa";
    std::string aA = "aA";
    std::string B = "B";

    file << static_cast<char>('a' + offset) << "\n";
    for (int i = 1; i <= 14; ++i) {
        file << static_cast<char>('a' + offset) + (Aa * i) << "\n";
    }
    //for (int i = 1; i <= 29; ++i) {
    //    *file << std::string(static_cast<char>('B' + offset), i) + 'a';
    //}
    for (int i = 0; i <= 28; ++i) {
        std::string curr = static_cast<char>('B' + offset) + std::string(i, 'B') + "a";
        file << curr << "\n";

        for (int j = 1; j <= (30 - i - 2) / 2; ++j) {
            std::string curr_with_zeros = curr;
            curr_with_zeros += Aa * j;
            file << curr_with_zeros << "\n";
            //if (curr_with_zeros.size() > 30) {
            //    throw new std::out_of_range("");
            //}
        }
    }

    for (int i = 0; i <= 13; ++i) {
        std::string curr = static_cast<char>('a' + offset) + std::string(1, 'A');
        curr += aA * i;
        for (int j = 1; j <= 30 - (i*2 + 2) - 1; ++j) {
            std::string curr_with_bs = curr;
            curr_with_bs += B * j;
            file << curr_with_bs + 'a' << "\n";
            //if (curr_with_bs.size() + 1 > 30) {
            //    throw new std::out_of_range("");
            //}
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie();
    Hasher h;

//    std::ofstream file("crackme2");
//    for (int i = 0; i <= 20; ++i) {
//        write(file, 24);
//    }

//    write(file, 0);
//    for (int i = 0; i < 8550; ++i) {
//        file << "aAaAaAaAaAaAaAaAaAaAaAaAaAaABa" << "\n";
//    }

//    for (int i = 0; i < 9000; ++i) {
//        file << genRandom(28) + "00" << "\n";
//    }

//    file.close();

    main2();
    return 0;
}