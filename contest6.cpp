#include <iostream>
#include <string>
#include <cmath>

enum class Operation { plus_mod, byte_xor };

static int module_number;

class UnrolledLinkedList {
    const int k_minimum_size_of_array_ = 6;

    static int array_max_length;
    static int half_of_array_length;
    static Operation selected_operation;

    // Выполняет операцию, основываясь на статическом selected_operation.
    static int executeSelectedOperation(int lhs, int rhs) {
        int result;
        switch (selected_operation) {
            case Operation::plus_mod:
                result = (lhs + (rhs) % module_number) % module_number;
                return (result > 0 ? result : result + module_number);
            case Operation::byte_xor:
                return lhs ^ rhs;
            default:
                throw std::exception();
        }
    }

    class Node {
        friend class UnrolledLinkedList;
        int operation_result_;
        int len_;
        int* values_;
        Node* next_ptr_ = nullptr;

        void updateOperationResult() {
            operation_result_ = 0;
            for (int i = 0; i < len_; ++i) {
                operation_result_ = executeSelectedOperation(operation_result_, values_[i]);
            }
        }

    public:
        // При создании нового нода высчитывается хранимый результат операций.
        Node(int* values, int len) {
            operation_result_ = 0;
            values_ = values;
            len_ = len;
            updateOperationResult();
        }

        void printToConsole() {
            // Выводит список, разделенный на массивы.
            std::cout << "len=" << len_ << " | ";
            for (int i = 0; i < len_; ++i) {
                std::cout << values_[i] << " ";
            }
            std::cout << "\n";
        }

        // Разделяет заполненный нод на 2 части и возвращает созданный (правый) нод.
        Node* splitNodeIntoTwo() {
            // Создание нового массива, в нем будет half элементов.
            int* new_array = new int[array_max_length];
            for (int i = 0; i < half_of_array_length; ++i) {
                new_array[i] = values_[len_ - half_of_array_length + i];
            }
            len_ = len_ % 2 == 0 ? half_of_array_length : half_of_array_length + 1;

            // В прошлом массиве нужно пересчитать хранимый результат операций.
            updateOperationResult();

            // Создание нового нода и его привязка.
            Node* new_node = new Node(new_array, half_of_array_length);
            new_node->next_ptr_ = next_ptr_;
            next_ptr_ = new_node;
            return new_node;
        }

        void insert(int value, int position_in_array) {
            if (len_ == array_max_length) {
                // Разделение нода на 2 части.
                Node* new_node = splitNodeIntoTwo();

                // Вставка элемента.
                if (position_in_array > len_) {
                    new_node->insert(value, position_in_array - len_);
                } else {
                    insert(value, position_in_array);
                }
            } else {
                // Перенос значений в массиве на один вправо.
                for (int i = len_ - 1; i >= position_in_array - 1; --i) {
                    values_[i + 1] = values_[i];
                }
                values_[position_in_array - 1] = value;

                ++len_;
                // Изменить хранимый результат операции.
                operation_result_ = executeSelectedOperation(operation_result_, value);
            }
        }

        void erase(int position_in_array) {
            // Изменить хранимый результат операции.
            switch (selected_operation) {
                case Operation::plus_mod:
                    operation_result_ = executeSelectedOperation(
                        operation_result_, module_number - values_[position_in_array - 1]);
                    break;
                case Operation::byte_xor:
                    operation_result_ =
                        executeSelectedOperation(operation_result_, values_[position_in_array - 1]);
                    break;
            }
            // Перенос значений на один влево.
            for (int i = position_in_array - 1; i < len_ - 1; ++i) {
                values_[i] = values_[i + 1];
            }
            --len_;
        }

        void change(int value, int position_in_array) {
            // Изменить хранимый результат операции.
            // Сначала вычитание прошлого, потом сложение с новым.
            switch (selected_operation) {
                case Operation::plus_mod:
                    operation_result_ = executeSelectedOperation(
                        operation_result_,
                        (module_number - values_[position_in_array - 1]) % module_number + value);
                    break;
                case Operation::byte_xor:
                    operation_result_ = executeSelectedOperation(
                        executeSelectedOperation(operation_result_, values_[position_in_array - 1]),
                        value);
                    break;
            }
            // Изменить значение.
            values_[position_in_array - 1] = value;
        }

        ~Node() {
            delete[] values_;
        }
    };

    Node* head_ = nullptr;
    // tail_for_reading_ только для комфорта изначального заполнения.
    Node* tail_for_reading_ = nullptr;

    void pushNodeBack(int* array, int len) {
        if (head_ == nullptr) {
            head_ = new Node(array, len);
            tail_for_reading_ = head_;
        } else {
            tail_for_reading_->next_ptr_ = new Node(array, len);
            tail_for_reading_ = tail_for_reading_->next_ptr_;
        }
    }

    void deleteNode(Node* prev_node, Node* deletable_node) {
        if (prev_node == nullptr) {
            // Значит - удаляется нод head_.
            head_ = head_->next_ptr_;
            delete deletable_node;
        } else {
            prev_node->next_ptr_ = deletable_node->next_ptr_;
            delete deletable_node;
        }
    }

    Node* findNodeByPosition(int position, int* position_in_array, Node** prev_node) {
        Node* curr = head_;
        *prev_node = nullptr;
        int prev_number_of_passed_positions = 0;
        int number_of_passed_positions = curr->len_;

        while (number_of_passed_positions < position) {
            *prev_node = curr;
            curr = curr->next_ptr_;
            prev_number_of_passed_positions = number_of_passed_positions;
            number_of_passed_positions += curr->len_;
        }
        *position_in_array = position - prev_number_of_passed_positions;
        return curr;
    }

    // Возвращает хвостовой нод или nullptr, если его нет.
    Node* findTail() {
        if (head_ == nullptr) {
            return nullptr;
        }

        Node* curr = head_;
        while (curr->next_ptr_ != nullptr) {
            curr = curr->next_ptr_;
        }
        return curr;
    }

public:
    explicit UnrolledLinkedList(Operation op) {
        selected_operation = op;
    }

    // Сложность: O(N)
    void readFromConsole() {
        int elements_count;
        std::cin >> elements_count;

        array_max_length = static_cast<int>(sqrt(elements_count));
        // Длина массива не может быть меньше 6,
        // иначе затраты на один нод будут слишком большими, мне кажется.
        if (array_max_length < k_minimum_size_of_array_) {
            array_max_length = k_minimum_size_of_array_;
        }
        half_of_array_length = array_max_length / 2;
        int arrays_count = elements_count / half_of_array_length;
        int remain_length = elements_count % half_of_array_length;

        int* array;
        // Ввод всех массивов, кроме последнего.
        for (int i = 0; i < arrays_count; ++i) {
            array = new int[array_max_length];
            // Заполнение массива.
            for (int j = 0; j < half_of_array_length; ++j) {
                std::cin >> array[j];
            }
            pushNodeBack(array, half_of_array_length);
        }

        // Ввод последнего.
        if (remain_length != 0) {
            array = new int[array_max_length];
            for (int i = 0; i < remain_length; ++i) {
                std::cin >> array[i];
            }
            pushNodeBack(array, remain_length);
        }
    }

    void printToConsole() {
        Node* curr = head_;
        while (curr) {
            curr->printToConsole();
            curr = curr->next_ptr_;
        }
    }

    int sizeOfList() {
        Node* curr = head_;
        int size = 0;
        while (curr) {
            size += curr->len_;
            curr = curr->next_ptr_;
        }
        return size;
    }

    // Сложность: O(3*sqrt(N))
    void insert(int value, int position) {
        // В начале проверяются вырожденные случаи: вставка при отсутствии элементов и вставка в
        // конец. За O(sqrt(N)).
        int size = sizeOfList();
        if (size == 0 && position == 1) {
            pushNodeBack(new int[array_max_length]{value}, 1);
            return;
        }
        if (position == size + 1) {
            Node* tail = findTail();
            tail->insert(value, tail->len_ + 1);
            return;
        }

        // Дойти до нужного массива за O(sqrt(N)).
        int position_in_array;
        Node* prev_node;
        Node* curr = findNodeByPosition(position, &position_in_array, &prev_node);

        // Вставить в массив значение (возможно, разделить массив на два) за O(sqrt(N)).
        curr->insert(value, position_in_array);
    }

    // Сложность: O(sqrt(N))
    void erase(int position) {
        // Дойти до нужного массива за O(sqrt(N)).
        int position_in_array;
        Node* prev_node;
        Node* curr = findNodeByPosition(position, &position_in_array, &prev_node);

        // Удалить элемент (или удалить нод, если это единственный элемент) за O(1).
        if (curr->len_ == 1) {
            deleteNode(prev_node, curr);
        } else {
            curr->erase(position_in_array);
        }
    }

    // Сложность: O(sqrt(N))
    void change(int value, int position) {
        // Дойти до нужного массива за O(sqrt(N)).
        int position_in_array;
        Node* prev_node;
        Node* curr = findNodeByPosition(position, &position_in_array, &prev_node);

        curr->change(value, position_in_array);
    }

    // Сложность: O(sqrt(N))
    int at(int position) {
        // Дойти до нужного массива за O(sqrt(N)).
        int position_in_array;
        Node* prev_node;
        Node* curr = findNodeByPosition(position, &position_in_array, &prev_node);

        return curr->values_[position_in_array - 1];
    }

    // Сложность: O(sqrt(N))
    // У каждого нода хранится значение выбранной операции для всего его внутреннего массива.
    // При каждой операции (insert, erase, change) изменяется значение на всем массиве.
    int compute(int left, int right) {
        // В этом методе можно делать оптимизации.
        // Например, не делать вычисления, если left - начало массива, а right - его конец.
        // Но я не уверен, что это так нужно.
        int result = 0;

        // Дойти до нужного нода за O(sqrt(N)).
        int position_of_left_in_array;
        Node* prev_left_node;
        Node* left_node = findNodeByPosition(left, &position_of_left_in_array, &prev_left_node);
        int position_of_right_in_array;
        Node* prev_right_node;
        Node* right_node = findNodeByPosition(right, &position_of_right_in_array, &prev_right_node);

        // Вариант, когда left и right находятся в одном ноде:
        if (left_node == right_node) {
            for (int i = position_of_left_in_array - 1; i < position_of_right_in_array; ++i) {
                result = executeSelectedOperation(result, left_node->values_[i]);
            }
        } else {
            // Более вероятный вариант:

            // Подсчет левой части.
            for (int i = position_of_left_in_array - 1; i < left_node->len_; ++i) {
                result = executeSelectedOperation(result, left_node->values_[i]);
            }

            // Подсчет средней части (operation_result из нодов).
            Node* middle_node = left_node->next_ptr_;
            while (middle_node != right_node) {
                result = executeSelectedOperation(result, middle_node->operation_result_);
                middle_node = middle_node->next_ptr_;
            }

            // Подсчет правой части.
            for (int i = 0; i < position_of_right_in_array; ++i) {
                result = executeSelectedOperation(result, right_node->values_[i]);
            }
        }
        return result;
    }

    ~UnrolledLinkedList() {
        Node* prev_node;
        while (head_) {
            prev_node = head_;
            head_ = head_->next_ptr_;
            delete prev_node;
        }
    }
};

// Статические переменные класса UnrolledLinkedList.
int UnrolledLinkedList::array_max_length;
int UnrolledLinkedList::half_of_array_length;
Operation UnrolledLinkedList::selected_operation;

void handleCommandsFromConsole(UnrolledLinkedList* list) {
    int commands_count;
    std::cin >> commands_count;
    std::string command;
    int value, position;
    for (int i = 0; i < commands_count; ++i) {
        std::cin >> command;
        if (command == "insert") {
            std::cin >> position >> value;
            list->insert(value, position);
        } else if (command == "erase") {
            std::cin >> position;
            list->erase(position);
        } else if (command == "change") {
            std::cin >> position >> value;
            list->change(value, position);
        } else if (command == "at") {
            std::cin >> position;
            std::cout << list->at(position) << "\n";
        } else if (command == "compute") {
            int left, right;
            std::cin >> left >> right;
            std::cout << list->compute(left, right) << "\n";
        } else if (command == "size") {
            std::cout << list->sizeOfList() << "\n";
        } else {
            throw std::exception();
        }
    }
}

Operation selectOperation() {
    char op;
    std::cin >> op;
    switch (op) {
        case '+':
            std::cin >> module_number;
            return Operation::plus_mod;
        case '^':
            return Operation::byte_xor;
        default:
            throw std::exception();
    }
}

void startTests() {
    // Питон.
    // freopen("tests.txt", "r", stdin);

    for (int i = 0; i < 10; ++i) {
        UnrolledLinkedList list(selectOperation());
        list.readFromConsole();
        handleCommandsFromConsole(&list);
    }

    exit(0);
}

int main() {
    // startTests();

    UnrolledLinkedList list(selectOperation());
    list.readFromConsole();

    handleCommandsFromConsole(&list);

    // list.printToConsole();
    return 0;
}
