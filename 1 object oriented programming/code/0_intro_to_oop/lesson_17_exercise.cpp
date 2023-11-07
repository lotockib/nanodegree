#include <cassert>
#include <stdexcept>

// TODO: Define class Pyramid
class Pyramid {
    public:
        Pyramid(int l, int w, int h) {
            Length(l);
            Width(w);
            Height(h);
            Validate();
        }
        int Length() {
            return length;
        }
        void Length(int l) {
            if ( l > 0 )
                length = l;
            CalculateVolume();
            }

        int Width() {
            return width;
        }
        void Width(int w) {
            if ( w > 0 )
                width = w;
            CalculateVolume();
            }

        int Height() {
            return height;
        }
        void Height(int h) {
            if ( h > 0 )
                height = h;
            CalculateVolume();
            }

        float Volume() {
            return volume;
        }

    private:
        int length;
        int width;
        int height;
        float volume;

        void CalculateVolume() {
            volume = length*width*height / 3;
        }

        void Validate() {
            if ( width <= 0 || height <= 0 || length <= 0 )
                throw std::invalid_argument("negative dimension");
        }


};

// Test
int main() {
  Pyramid pyramid(4, 5, 6);
  assert(pyramid.Length() == 4);
  assert(pyramid.Width() == 5);
  assert(pyramid.Height() == 6);
  assert(pyramid.Volume() == 40);

  bool caught{false};
  try {
    Pyramid invalid(-1, 2, 3);
  } catch (...) {
    caught = true;
  }
  assert(caught);
}