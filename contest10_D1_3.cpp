// CPP program to illustrate the
// unordered_set::hash() function
  
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
  
using namespace std;

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

std::size_t hash_range(std::string::iterator first, std::string::iterator last)
{
    std::size_t seed = 0;

    for(; first != last; ++first)
    {
        hash<char> hasher;
        seed ^= hasher(*first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }

    return seed;
}

size_t hashString(string str) {
    return hash_range(str.begin(), str.end());
}

int main()
{
    string me = "a";
    hash<char> hasher_char;
    hash<string> hasher_str;
    cout << hasher_char(*me.begin()) << "\n";
    cout << hasher_str(me) << "\n";
    cout << hashString(me) << "\n";

    /*
    unordered_set<string> sample_set(23);

    // use of hash_function
    unordered_set<string>::hasher fn = sample_set.hash_function();
  
    //cout << fn("geeks") << endl;
    std::hash<string> hasher;

    string initial_str = "vOU2UzgyUp08wq9";
    size_t initial_hash = fn(initial_str) % sample_set.bucket_count();

    std::vector<string> hashes;
    hashes.push_back(initial_str);

    string curr;
    bool new_hash_found;
    for (int i = 0; i < 14999; ++i) {
        new_hash_found = false;
        while (!new_hash_found) {
            curr = genRandom(15);
            if (fn(curr) % sample_set.bucket_count() == initial_hash && std::find(hashes.begin(), hashes.end(), curr) == hashes.end()) {
                //cout << "found!\n";
                hashes.push_back(curr);
                new_hash_found = true;
            }
        }
    }

    ofstream file("crackme");
    for (auto it = hashes.begin(); it != hashes.end(); it++) {
        file << *it << "\n";
        cout << *it << "\n";
    }
    for (auto it = hashes.begin(); it != hashes.end(); it++) {
        cout << fn(*it) % sample_set.bucket_count() << "\n";
    }
    cout << endl;
     */
    //cout << fn("vOU2UzgyUp08wq9") % sample_set.bucket_count() << "\n";
    //cout << fn("fa37JncCHryDsbz") % sample_set.bucket_count() << "\n";
    //cout << sample_set.bucket_count();
    //cout << fn("ayy4cBWDxS22Jjz") % sample_set.bucket_count() << "\n";
    //cout << fn("hMaiRrV41mtzxlY") % sample_set.bucket_count() << "\n";
    //cout << fn("gNYawRIW7fjQ5Hi") << "\n";
    //cout << fn("WfT0FdrXmebEP05") << "\n";
    //cout << fn("CQPb2K45vBr4vW1") << "\n";

    return 0;
}