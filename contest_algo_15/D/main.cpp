#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>
#include <iomanip>

struct Point {
    double x;
    double y;

    Point() {
    }

    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    void readDataFromConsole() {
        std::cin >> x >> y;
    }

    bool operator==(const Point &other) {
        return x == other.x && y == other.y;
    }
};

struct Circle {
    double radius;
    Point center;

    void readDataFromConsole() {
        center.readDataFromConsole();
        std::cin >> radius;
    }

    bool operator==(const Circle &other) {
        return radius == other.radius && center == other.center;
    }
};

// Вычисляет длину вектора.
double vectorLength(const Point &begin, const Point &end) {
    return sqrt(pow(begin.x - end.x, 2) + pow(begin.y - end.y, 2));
}

// Определяет равенство для вещественных чисел.
bool doubleEquals(const double &first, const double &second) {
    // Эпсилон = 10^(-7)
    const double k_epsilon = 0.0000001;
    return fabs(first - second) < k_epsilon;
}

// Возвращает 0, 1 или 2 точки пересечения между двумя кругами.
std::vector<Point> getIntersectionPoints(Circle c_1, Circle c_2) {
    std::vector<Point> intersection;

    // Вывод формул проводился на листочке.

    // Считаем, что первая окружность лежит в начале координат.
    // Тогда:
    // a - это X-координата центра второй окружности относительно первой.
    // b - это Y-координата центра второй окружности относительно первой.
    double r_1 = c_1.radius;
    double r_2 = c_2.radius;
    double a = c_2.center.x - c_1.center.x;
    double b = c_2.center.y - c_1.center.y;

    if (a == 0 && b == 0) {
        // Если у окружностей центры в разных точках.
        if (r_1 == r_2) {
            throw std::exception();
        }
    } else if (a == 0) {
        // Если центры окружностей стоят на одной вертикальной оси.
        double y = (pow(b, 2) - pow(r_2, 2) + pow(r_1, 2)) / (2 * b);
        double x = sqrt(pow(r_1, 2) - pow(y, 2));
        if (x == 0) {
            intersection.push_back(Point(x, y));
        } else if (x == x) {
            intersection.push_back(Point(x, y));
            intersection.push_back(Point(-x, y));
        }
    } else if (b == 0) {
        // Если центры окружностей стоят на одной горизонтальной оси.
        double x = (pow(a, 2) - pow(r_2, 2) + pow(r_1, 2)) / (2 * a);
        double y = sqrt(pow(r_1, 2) - pow(x, 2));
        if (y == 0) {
            intersection.push_back(Point(x, y));
        } else if (y == y) {
            intersection.push_back(Point(x, y));
            intersection.push_back(Point(x, -y));
        }
    } else {
        // Общий случай, центры окружностей никак не связаны, a != 0, b != 0.

        // Константа для вычислений.
        double const_1 = pow(a, 2) + pow(b, 2) - pow(r_2, 2) + pow(r_1, 2);

        // Коэффициенты в квадратичном уравнении.
        double a_coef = 1 + 4 * pow(a, 2) / (4 * pow(b, 2));
        double b_coef = -4 * a * const_1 / (4 * pow(b, 2));
        double c_coef = pow(const_1, 2) / (4 * pow(b, 2)) - pow(r_1, 2);

        double discriminant = pow(b_coef, 2) - 4 * a_coef * c_coef;

        if (discriminant == 0) {
            double x = -b_coef / (2 * a_coef);
            double y = (-2 * a * x + const_1) / (2 * b);
            intersection.push_back(Point(x, y));
        } else if (discriminant > 0) {
            double x1 = (-b_coef + sqrt(discriminant)) / (2 * a_coef);
            double x2 = (-b_coef - sqrt(discriminant)) / (2 * a_coef);
            double y1 = (-2 * a * x1 + const_1) / (2 * b);
            double y2 = (-2 * a * x2 + const_1) / (2 * b);
            intersection.push_back(Point(x1, y1));
            intersection.push_back(Point(x2, y2));
        }
    }

    // Здесь все точки сдвигаются на координаты центра первой окружности.
    for (size_t i = 0; i < intersection.size(); ++i) {
        intersection[i].x += c_1.center.x;
        intersection[i].y += c_1.center.y;
        //        std::cout << "( " << intersection[i].x << " ; " << intersection[i].y << " )"
        //                  << "\n";
    }
    return intersection;
}

// Длина дуги между двумя точками.
double arcLength(Point p_1, Point p_2, Circle circle) {
    double v_1_x = p_1.x - circle.center.x;
    double v_2_x = p_2.x - circle.center.x;
    double v_1_y = p_1.y - circle.center.y;
    double v_2_y = p_2.y - circle.center.y;
    double angle_cos = (v_1_x * v_2_x + v_1_y * v_2_y) /
                       (vectorLength(p_1, circle.center) * vectorLength(p_2, circle.center));
    double angle_radians = acos(angle_cos);
    double angle_degrees = angle_radians * 180 / M_PI;
    double arc_length = (2 * M_PI * circle.radius * angle_degrees) / 360;
    return arc_length;
}

// Определяет, принадлежит ли точка окружности.
bool pointBelongsToCircle(Point point, Circle circle) {
    return doubleEquals(vectorLength(circle.center, point), circle.radius);
}

// Создает граф из 2, 3 или 4 точек.
// точка 0 - это точка начала
// точка 1 - это первая точка пересечения
// точка 2 - это вторая точка пересечения
// точка 3 - это точка конца
std::vector<std::vector<std::pair<int, double>>> createGraphWithPointsOnCircles(Circle c_1,
                                                                                Circle c_2,
                                                                                Point p_begin,
                                                                                Point p_end) {
    std::vector<std::vector<std::pair<int, double>>> graph(4);

    auto add_two_arcs = [&](const int &from, const int &to, const double &weight) {
        graph[from].push_back({to, weight});
        graph[to].push_back({from, weight});
    };

    std::vector<Point> intersection = getIntersectionPoints(c_1, c_2);

    Circle c_begin = pointBelongsToCircle(p_begin, c_1) ? c_1 : c_2;
    Circle c_end = pointBelongsToCircle(p_end, c_1) ? c_1 : c_2;

    if (intersection.size() == 1) {
        add_two_arcs(0, 1, arcLength(p_begin, intersection[0], c_begin));
        add_two_arcs(3, 1, arcLength(p_end, intersection[0], c_end));
        // Если начало и конец стоят на одной окружности, их тоже нужно соединить дугами.
        if (c_begin == c_end) {
            add_two_arcs(0, 3, arcLength(p_begin, p_end, c_begin));
        }
    } else if (intersection.size() == 2) {
        add_two_arcs(0, 1, arcLength(p_begin, intersection[0], c_begin));
        add_two_arcs(3, 1, arcLength(p_end, intersection[0], c_end));

        add_two_arcs(0, 2, arcLength(p_begin, intersection[1], c_begin));
        add_two_arcs(3, 2, arcLength(p_end, intersection[1], c_end));

        add_two_arcs(1, 2, arcLength(intersection[0], intersection[1], c_begin));

        if (c_begin == c_end) {
            add_two_arcs(0, 3, arcLength(p_begin, p_end, c_begin));
        }
    }

    return graph;
}

class Dijkstra {
private:
    int number_of_vertices_;
    std::vector<std::vector<std::pair<int, double>>> graph_;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>,
                        std::greater<std::pair<double, int>>>
        queue_;
    std::vector<double> distance_;

public:
    void setGraph(const std::vector<std::vector<std::pair<int, double>>> &graph) {
        number_of_vertices_ = 4;
        graph_ = graph;
    }

    void calculateDistances() {
        distance_.resize(number_of_vertices_, -1);
        distance_[0] = 0;
        queue_.push({0, 0});
        while (!queue_.empty()) {
            double weight = queue_.top().first;
            int index_of_min = queue_.top().second;
            queue_.pop();
            if (weight > distance_[index_of_min]) {
                continue;
            }

            for (const std::pair<int, double> &child : graph_[index_of_min]) {
                double new_distance = distance_[index_of_min] + child.second;
                if (new_distance < distance_[child.first] || distance_[child.first] == -1) {
                    distance_[child.first] = new_distance;
                    queue_.push({new_distance, child.first});
                }
            }
        }
    }

    double getDistanceToLastVertex() {
        return distance_.back();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Circle c_1, c_2;
    Point p_begin, p_end;
    c_1.readDataFromConsole();
    c_2.readDataFromConsole();
    p_begin.readDataFromConsole();
    p_end.readDataFromConsole();

    auto graph = createGraphWithPointsOnCircles(c_1, c_2, p_begin, p_end);

    Dijkstra algo;
    algo.setGraph(graph);
    algo.calculateDistances();
    std::cout << std::fixed << std::setprecision(10) << algo.getDistanceToLastVertex();
    return 0;
}
