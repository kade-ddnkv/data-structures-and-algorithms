#include <iostream>
#include <unordered_set>
#include <ctime>
#include <unistd.h>
#include <fstream>

#include <string>
#include <cctype>
#include <random>
#include <cstdlib>

std::string gen_random(const int len) {
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


struct BucketStats {
    size_t occupied = 0;
    size_t total_collisions = 0;
    size_t max_collisions = 0;
    //std::unordered_set<int>::size_type max_bucket;
    int max_bucket;

    template<class... Args>
    BucketStats(std::unordered_set<Args...> const& c)
    {
        for(auto bucket = c.bucket_count(); bucket--;) {
            auto bucket_size = c.bucket_size(bucket);
            occupied += bucket_size > 0;
            if (bucket_size > 1) {
                auto collisions = bucket_size - 1;
                total_collisions += collisions;
                max_collisions = std::max(max_collisions, collisions);
            }
            if (bucket_size == max_collisions) {
                max_bucket = bucket;
            }
        }
        auto fn = c.hash_function();
        auto begin = c.begin(max_bucket);
        auto end = c.end(max_bucket);
        std::cout << "\nmax collision bucket:\n";
        while (begin != end) {
            std::cout << *begin << "\n";
            begin++;
        }
        begin = c.begin(max_bucket);
        std::cout << "\ntheir hashes:\n";
        while (begin != end) {
            std::cout << fn(*begin) % c.bucket_count() << "\n";
            begin++;
        }
        std::cout << "\n";
    }

    double avg_collisions() const {
        return occupied ? static_cast<double>(total_collisions) / occupied : 0;
    }

    friend std::ostream& operator<<(std::ostream& s, BucketStats const& b) {
        return s
               << "used buckets: " << b.occupied
               << "; total collisions: " << b.total_collisions
               << "; max collisions in a bucket: " << b.max_collisions
               << "; avg collisions per bucket: " << b.avg_collisions()
            ;
    }
};

int main()
{
    srand(static_cast<unsigned>(time(nullptr)) * getpid());
    std::unordered_set<std::string> set;
    int c = 150000;

    std::ofstream file("crackme");
    std::string curr;

    for (int i = 0; i < c; ++i) {
        curr = gen_random(15);
        file << curr << "\n";
        set.insert(curr);
    }

    std::cout << "insert ended" << std::endl;

    std::cout << BucketStats(set) << '\n';
    return 0;
}

/*
#include <iostream>
#include <fstream>

void generate() {
    std::ofstream file("crashme");
    for (int i = 0; i < 15000; ++i) {
        file << "user" << i << "\n";
    }
    file.close();
}

int main() {
    generate();
    return 0;
}
*/