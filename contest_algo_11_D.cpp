#include <iostream>
#include <vector>
#include <algorithm>

void readData(std::vector<int> *rooms_lts, std::vector<int> *rooms_connections) {
    int number_of_rooms;
    std::cin >> number_of_rooms;
    rooms_lts->resize(number_of_rooms);
    rooms_connections->resize(number_of_rooms - 1);
    for (int i = 0; i < number_of_rooms; ++i) {
        std::cin >> (*rooms_lts)[i];
    }
    for (int i = 0; i < number_of_rooms - 1; ++i) {
        std::cin >> (*rooms_connections)[i];
    }
}

int bringInternetToRooms(const std::vector<int> &rooms_lts,
                         const std::vector<int> &rooms_connections) {
    int number_of_rooms = rooms_lts.size();
    int division_cost = rooms_lts[0];
    int full_cost = rooms_lts[0];
    for (int room_i = 1; room_i < number_of_rooms; ++room_i) {
        bool action_is_lts = true;
        int curr_cost = rooms_lts[room_i];
        if (rooms_lts[room_i] + rooms_connections[room_i - 1] - division_cost < curr_cost) {
            curr_cost = rooms_lts[room_i] + rooms_connections[room_i - 1] - division_cost;
        }
        if (rooms_connections[room_i - 1] < curr_cost) {
            curr_cost = rooms_connections[room_i - 1];
            action_is_lts = false;
        }

        if (action_is_lts) {
            division_cost = rooms_lts[room_i];
        } else if (division_cost < rooms_connections[room_i - 1]) {
            division_cost = rooms_connections[room_i - 1];
        }
        full_cost += curr_cost;
    }
    return full_cost;
}

int main() {
    std::vector<int> rooms_lts, rooms_connections;
    readData(&rooms_lts, &rooms_connections);
    std::cout << bringInternetToRooms(rooms_lts, rooms_connections) << "\n";
    return 0;
}
