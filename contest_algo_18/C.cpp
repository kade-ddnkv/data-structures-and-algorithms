#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class Variations {
private:
    int n_objects_, n_places_;
    std::vector<int> var_;

    void nextVariation() {
        int i = n_places_ - 1;
        while (var_[i] == n_objects_ - 1) {
            --i;
        }
        ++var_[i];
        for (int j = i + 1; j < n_places_; ++j) {
            var_[j] = 0;
        }
    }

    void print() {
        for (size_t i = 0; i < var_.size() - 1; ++i) {
            std::cout << var_[i] + 1 << " ";
        }
        std::cout << var_.back() + 1 << "\n";
    }

public:
    Variations(int n_objects, int n_places) {
        n_objects_ = n_objects;
        n_places_ = n_places;
        var_.resize(n_places_);
    }

    void printVariationByIndex(int variation_i) {
        for (int i = 0; i < variation_i; ++i) {
            nextVariation();
        }
        print();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n_tracks, n_places, variation_i;
    std::cin >> n_tracks >> n_places >> variation_i;
    Variations algo(n_tracks, n_places);
    algo.printVariationByIndex(variation_i - 1);
    return 0;
}
