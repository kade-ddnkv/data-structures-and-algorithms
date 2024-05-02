#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <random>

std::string genRandom(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

int main()
{
    //std::ios_base::sync_with_stdio(false);
    //std::mt19937 gen(time(nullptr));
    //std::uniform_int_distribution<int> dist(30'000, 1'000'000);
    //size_t init_size = dist(gen);

    //std::cout << init_size << "\n";

    //unordered_set<string> sample_set(23);

    // use of hash_function
    //unordered_set<string>::hasher fn = sample_set.hash_function();

    //cout << fn("geeks") << endl;
    //std::hash<string> hasher;

    //string initial_str = "vOU2UzgyUp08wq9";
    //size_t initial_hash = fn(initial_str) % sample_set.bucket_count();

    //std::vector<std::string> hashes;
    std::hash<std::string> hasher;
    //std::string initial_str = "aaaaaaaa";
    //size_t initial_hash = hasher(initial_str);
    //hashes.push_back(initial_str);

    std::string init_str = "aaaaaaaa";
    //size_t init_hash = hasher(init_str + "A");
    char achar = 'A';
    std::cout << hasher(init_str + achar) << "\n";
    achar += 1;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 2;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 4;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 8;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 16;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 32;
    std::cout << hasher(init_str + achar) << "\n";
    achar += 64;
    std::cout << hasher(init_str + achar) << "\n";


    //std::cout << hasher(init_str + "D") << "\n";
    //std::cout << hasher(init_str + "H") << "\n";
    //for (int i = 1; i < 10; ++i) {
    //    if (hasher(init_str + std::to_string(i)) == init_hash) {
    //        std::cout << init_str + std::to_string(i);
    //    }
    //}

    /*
    std::string curr;
    bool new_hash_found;
    for (int i = 0; i < 3; ++i) {
        new_hash_found = false;
        while (!new_hash_found) {
            curr = genRandom(8);
            if (hasher(curr) == initial_hash && curr != initial_str) {
                std::cout << curr << "\n";
                //hashes.push_back(curr);
                new_hash_found = true;
            }
        }
    }
     */

    //std::ofstream file("crackme");
    //for (auto it = hashes.begin(); it != hashes.end(); it++) {
    //    file << *it << "\n";
    //    std::cout << *it << "\n";
    //}
    //for (auto it = hashes.begin(); it != hashes.end(); it++) {
    //    std::cout << fn(*it) % sample_set.bucket_count() << "\n";
    //}
    std::cout << "\n";

    return 0;
}