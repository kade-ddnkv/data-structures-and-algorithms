#include <iostream>

class Cages {
    int* head_;
    int* tail_;
    // Для вывода в последовательность.
    int* next_;

public:
    explicit Cages(int kittens_number) {
        head_ = new int[kittens_number];
        tail_ = new int[kittens_number];
        next_ = new int[kittens_number];
        for (int i = 0; i < kittens_number; ++i) {
            head_[i] = i;
            tail_[i] = i;
            next_[i] = i;
        }
    }

    ~Cages() {
        delete[] head_;
        delete[] tail_;
        delete[] next_;
    }

    void removeWall(int left, int right) {
        int left_tail = getTail(left);
        int right_head = getHead(right);
        tail_[left_tail] = tail_[right_head];
        head_[right_head] = head_[left_tail];
        // Цепляю голову right в хвост left.
        next_[left_tail] = right_head;
    }

    int getHead(int cage) {
        if (head_[cage] == cage) {
            return cage;
        } else {
            // Привязка элемента к корню при обращении.
            head_[cage] = head_[head_[cage]];
            return getHead(head_[cage]);
        }
    }

    int getTail(int cage) {
        if (tail_[cage] == cage) {
            return cage;
        } else {
            // Привязка элемента к хвосту при обращении.
            tail_[cage] = tail_[tail_[cage]];
            return getTail(tail_[cage]);
        }
    }

    void printOriginalAviary() {
        int curr = getHead(0);
        while (curr != next_[curr]) {
            std::cout << curr + 1 << " ";
            curr = next_[curr];
        }
        std::cout << curr + 1;
    }
};

void handleConsoleCommands(Cages* cages, int kittens_number) {
    int left_kitten, right_kitten;

    for (int i = 0; i < kittens_number - 1; ++i) {
        std::cin >> left_kitten >> right_kitten;
        cages->removeWall(left_kitten - 1, right_kitten - 1);
    }
}

int main() {
    int kittens_number;
    std::cin >> kittens_number;
    Cages cages(kittens_number);

    handleConsoleCommands(&cages, kittens_number);
    cages.printOriginalAviary();
    return 0;
}
