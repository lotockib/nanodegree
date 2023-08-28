#include <cassert>
#include <cmath>
#include <stdexcept>

// For a static method, don't need any actual class data and methods, or to instantiate a class object
class Sphere {
 public:
    static float Volume(int radius) {
        return pi_ * 4 / 3 * pow(radius, 3);
    }
 private:
  static float constexpr pi_{3.14159};
};

// Test
int main(void) {
  assert(abs(Sphere::Volume(5) - 523.6) < 1);
}