    #include <iostream>
    #include <vector>
    #include <string>
    #include <algorithm>

    std::vector<int> queue;
    int queue_index = 0;
    int index_to_flush;
    std::vector<int> used;

    std::vector<std::string> names;

    std::vector<std::vector<int>> graph;

    int readData() {
        int number_of_persons, number_of_connections;
        std::cin >> number_of_persons >> number_of_connections;
        graph.resize(number_of_persons, std::vector<int>());
        used.resize(number_of_persons);
        names.resize(number_of_persons);
        for (int i = 0; i < number_of_persons; ++i) {
            int person_number;
            std::string name, surname;
            std::cin >> person_number;
            std::getline(std::cin, name);
            names[person_number] = name.substr(1);
        }
        for (int i = 0; i < number_of_connections; ++i) {
            int left;
            int right;
            std::cin >> left >> right;
            graph[left].push_back(right);
            graph[right].push_back(left);
        }
        int start_index;
        std::cin >> start_index;
        return start_index;
    }

    void flush() {
        std::vector<std::string> local_names;
        for (size_t i = queue_index + 1; i < queue.size(); ++i) {
            local_names.push_back(names[queue[i]]);
        }
        std::sort(local_names.begin(), local_names.end());
        for (const std::string &name : local_names) {
            std::cout << name << "\n";
        }
    }

    void bfsWithFlush(int start_index) {
        index_to_flush = start_index;
        used[start_index] = 1;
        queue.push_back(start_index);
        std::cout << names[start_index] << "\n";
        while (queue_index != static_cast<int>(queue.size())) {
            int queue_top = queue[queue_index];
            std::vector<int> &curr_connections = graph[queue_top];
            for (size_t i = 0; i < curr_connections.size(); ++i) {
                if (used[curr_connections[i]] == 0) {
                    queue.push_back(curr_connections[i]);
                    used[curr_connections[i]] = 1;
                }
            }
            if (queue_top == index_to_flush) {
                flush();
                index_to_flush = queue.back();
            }
            ++queue_index;
        }
    }

    int main() {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        int start_index = readData();
        bfsWithFlush(start_index);
        return 0;
    }
