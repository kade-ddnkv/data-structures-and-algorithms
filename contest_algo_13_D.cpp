#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

// Идея решения.
// Сначала простым методом подходим на расстояние около 9 клеток до финиша.
// Уже в этом маленьком квадрате запускаем bfs,
// который точно находит минимальный остаток пути до финиша.

struct Point {
    int x;
    int y;

    Point() {
        x = -1;
        y = -1;
    }

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool equals(Point other) const {
        return x == other.x && y == other.y;
    }
};

int size;
Point start;
Point finish;

void readData() {
    std::cin >> size;
    std::cin >> start.x >> start.y;
    std::cin >> finish.x >> finish.y;
}

int maxDistance(Point first, Point second) {
    return std::max(abs(first.x - second.x), abs(first.y - second.y));
}

std::vector<Point> next_points;

void makeNextPoints(Point curr) {
    next_points.clear();
    next_points.push_back(Point(curr.x - 2, curr.y - 1));
    next_points.push_back(Point(curr.x - 2, curr.y + 1));
    next_points.push_back(Point(curr.x - 1, curr.y - 2));
    next_points.push_back(Point(curr.x - 1, curr.y + 2));
    next_points.push_back(Point(curr.x + 1, curr.y - 2));
    next_points.push_back(Point(curr.x + 1, curr.y + 2));
    next_points.push_back(Point(curr.x + 2, curr.y - 1));
    next_points.push_back(Point(curr.x + 2, curr.y + 1));
}

void printBacktrace(Point curr) {
    std::vector<Point> path;
    path.push_back(curr);
    while (!curr.equals(start)) {
        int diff_x = abs(curr.x - start.x);
        int diff_y = abs(curr.y - start.y);
        if (diff_x == 0) {
            if (curr.x == 1) {
                curr.y += curr.y < start.y ? 2 : -2;
                curr.x += 1;
                continue;
            }
            if (curr.x == size) {
                curr.y += curr.y < start.y ? 2 : -2;
                curr.x += -1;
                continue;
            }
        }
        if (diff_y == 0) {
            if (curr.y == 1) {
                curr.x += curr.x < start.x ? 2 : -2;
                curr.y += 1;
                continue;
            }
            if (curr.y == size) {
                curr.x += curr.x < start.x ? 2 : -2;
                curr.y -= 1;
                continue;
            }
        }
        if (diff_x > diff_y) {
            curr.x += curr.x < start.x ? 2 : -2;
            curr.y += curr.y < start.y ? 1 : -1;
        } else {
            curr.x += curr.x < start.x ? 1 : -1;
            curr.y += curr.y < start.y ? 2 : -2;
        }
        path.push_back(curr);
    }
    for (int i = path.size() - 1; i >= 0; --i) {
        std::cout << path[i].x << " " << path[i].y << "\n";
    }
}

const int kMinBoxHalf = 9;
const int kMinBoxSide = kMinBoxHalf * 2 + 1;

int getXIndexInBox(int global_x) {
    return global_x - finish.x + kMinBoxHalf;
}

int getYIndexInBox(int global_y) {
    return global_y - finish.y + kMinBoxHalf;
}

void simpleTraceToFinishBox(int *first_backtrace_level, Point *curr) {
    *first_backtrace_level = 0;
    while (maxDistance(*curr, finish) > kMinBoxHalf) {
        ++(*first_backtrace_level);
        int diff_x = abs(curr->x - finish.x);
        int diff_y = abs(curr->y - finish.y);
        if (diff_x == 0) {
            if (curr->x == 1) {
                curr->y += curr->y < finish.y ? 2 : -2;
                curr->x += 1;
                continue;
            }
            if (curr->x == size) {
                curr->y += curr->y < finish.y ? 2 : -2;
                curr->x += -1;
                continue;
            }
        }
        if (diff_y == 0) {
            if (curr->y == 1) {
                curr->x += curr->x < finish.x ? 2 : -2;
                curr->y += 1;
                continue;
            }
            if (curr->y == size) {
                curr->x += curr->x < finish.x ? 2 : -2;
                curr->y -= 1;
                continue;
            }
        }
        if (diff_x > diff_y) {
            curr->x += curr->x < finish.x ? 2 : -2;
            curr->y += curr->y < finish.y ? 1 : -1;
        } else {
            curr->x += curr->x < finish.x ? 1 : -1;
            curr->y += curr->y < finish.y ? 2 : -2;
        }
    }
}

void solve() {
    std::vector<std::vector<bool>> used;
    used.resize(kMinBoxSide, std::vector<bool>(kMinBoxSide));
    std::vector<std::vector<int>> level;
    level.resize(kMinBoxSide, std::vector<int>(kMinBoxSide));
    std::vector<std::vector<Point>> parent;
    parent.resize(kMinBoxSide, std::vector<Point>(kMinBoxSide));

    Point curr = start;
    int first_backtrace_level;

    simpleTraceToFinishBox(&first_backtrace_level, &curr);
    if (curr.equals(finish)) {
        std::cout << first_backtrace_level << "\n";
        printBacktrace(finish);
        return;
    }
    Point nearest_point = curr;

    std::queue<Point> queue;
    queue.push(nearest_point);
    used[getXIndexInBox(nearest_point.x)][getYIndexInBox(nearest_point.y)] = true;

    // BFS
    while (!queue.empty()) {
        makeNextPoints(queue.front());
        for (const Point &next_point : next_points) {
            int box_x = getXIndexInBox(next_point.x);
            int box_y = getYIndexInBox(next_point.y);
            if (next_point.equals(finish)) {
                // Вывод пути.
                std::cout
                    << first_backtrace_level +
                           level[getXIndexInBox(queue.front().x)][getYIndexInBox(queue.front().y)] +
                           1
                    << "\n";
                printBacktrace(nearest_point);
                Point backtrace = queue.front();
                std::vector<Point> path;
                while (!backtrace.equals(nearest_point)) {
                    path.push_back(backtrace);
                    backtrace = parent[getXIndexInBox(backtrace.x)][getYIndexInBox(backtrace.y)];
                }
                for (int i = path.size() - 1; i >= 0; --i) {
                    std::cout << path[i].x << " " << path[i].y << "\n";
                }
                std::cout << finish.x << " " << finish.y << "\n";
                return;
            }
            // Проверки:
            // не вылезает ли за поле следующая вершина
            // находится ли вершина внутри коробочки
            // не использована ли еще вершина
            if (next_point.x >= 1 && next_point.y >= 1 &&
                maxDistance(next_point, finish) <= kMinBoxHalf && !used[box_x][box_y]) {
                queue.push(next_point);
                used[box_x][box_y] = true;
                level[box_x][box_y] =
                    level[getXIndexInBox(queue.front().x)][getYIndexInBox(queue.front().y)] + 1;
                parent[box_x][box_y] = queue.front();
            }
        }
        queue.pop();
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    readData();
    solve();
    return 0;
}
