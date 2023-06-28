#include <assert.h>

class Point {
    public:
        Point(int x, int y) : x(x), y(y) {}

        // Argument is only what's after the +, not before
        Point operator+(const Point& p2) {
            Point p3(0, 0);
            p3.x = x + p2.x;
            p3.y = y + p2.y;
            return p3;
        }

        int x;
        int y;
};

// Test in main()
int main() {
  Point p1(10, 5), p2(2, 4);
  Point p3 = p1 + p2; // An example call to "operator +";
  assert(p3.x == p1.x + p2.x);
  assert(p3.y == p1.y + p2.y);
}