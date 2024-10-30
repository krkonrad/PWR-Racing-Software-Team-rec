//Standardowe biblioteki C++ zapewniojące funkcje elementarne
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
struct Point {
    double x, y;
};

/* Funkcja obliczająca orientację trzech punktów
 funkcja zwraca int  i przyjmuje  jako argumenty 3 obiekty typu  Point , który jest zdefiniowany powyrzej w lini 8,9,10
*/
int orientation(Point p, Point q, Point r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); // Obliczamy kwadrat odległości między punktami aby sprawdzić czy nie są współliniowe
    if (val == 0) return 0;          // if zwraca 0 jeżeli punkty się pokrywają
    return (val > 0) ? 1 : 2;
}
// Funkcja porównująca dwa punkty według kąta względem punktu bazowego
Point p0;
bool compare(const Point& p1, const Point& p2) {
    int o = orientation(p0, p1, p2);
    if (o == 0)
        return (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y) <
               (p2.x - p0.x) * (p2.x - p0.x) + (p2.y - p0.y) * (p2.y - p0.y);
    return (o == 2);
}

/*     Sprawdzanie liczby punktów (if (n < 3) return {};):
        W tym miejscu kodu sprawdzamy, czy liczba punktów jest mniejsza niż 3.
        Aby znlaleść skrajne punkty zbioru, potrzeba co najmniej trzech punktów, z 2 lub 1 punktu nie da się stwożyć figury zamkniętej.

Dalsze działanie funkcji (przy n >= 3):

Jeśli liczba punktów jest większa lub równa 3, funkcja przechodzi do kolejnych etapów, które wykonują właściwe obliczenia skrajnych punktów.*/
std::vector<Point> convexHull(std::vector<Point>& points) {
    int n = points.size(); // wywołanie metody która przypisuje liczbe elementów do zmiennej n
    if (n < 3) return {};

    // Znajdź punkt o najmniejszej współrzędnej y (p0), w przypadku o tej samej współżędnej y wybierz punkt o mniejszym x
    int ymin = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[ymin].y || (points[i].y == points[ymin].y && points[i].x < points[ymin].x))
            ymin = i;
    }

    std::swap(points[0], points[ymin]);
    p0 = points[0];
//Sortowanie elementow w kontenerze points zgodnie  funkcja compare ktora zaczyna sie w lini 22
    std::sort(points.begin() + 1, points.end(), compare);

    // Usunięcie punktów współliniowych z p0
    int m = 1;
    for (int i = 1; i < n; i++) {
        while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
            i++;
        points[m] = points[i];
        m++;
    }

    if (m < 3) return {};  // mniej niż 3 punkty oznaczją linie prostą lub odcinej dlatego wielokąt nie istnieje

   //hull, który przechowuje obiekty typu Point. Następnie wstawia na stos trzy elementy points[0], points[1] i points[2], które będą pierwszymi elementami strukturyy
    std::stack<Point> hull;
    hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    for (int i = 3; i < m; i++) {
        while (hull.size() > 1) {
            Point top = hull.top();
            hull.pop();
            Point next_to_top = hull.top();
            if (orientation(next_to_top, top, points[i]) != 2)
                continue;
            hull.push(top);
            break;
        }
        hull.push(points[i]);
    }

    // Zbierz punkty do wektora
    std::vector<Point> hull_points;
    while (!hull.empty()) {
        hull_points.push_back(hull.top());
        hull.pop();
    }

    return hull_points;
}

int main() {
    std::ifstream file("points.txt"); //funkcja atwarcia pliku
    if (!file) {
        std::cerr << "Nie można otworzyć pliku.\n";
        return 1;
    }

    int n;
    file >> n;
    std::vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        file >> points[i].x >> points[i].y;
    }
    file.close();

    std::vector<Point> hull_points = convexHull(points);

    std::cout << "Otoczka wypukła:\n";
    for (const auto& point : hull_points) {
        std::cout << "(" << std::fixed << std::setprecision(1) << point.x << ", " << point.y << ")\n";
    }
    return 0;
}
