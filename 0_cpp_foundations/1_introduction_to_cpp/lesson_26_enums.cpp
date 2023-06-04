#include <iostream>
using std::cout;

int main() {
    enum class Color {white, black, red, blue};

    Color my_color = Color::black;

    if (my_color == Color::black) {
        cout << "My color is black" << std::endl;
    }
    else {
        cout << "My color is not black" << std::endl;
    }

    // Switch example
    enum class Direction {kUp, kDown, kLeft, kRight};
    Direction a = Direction::kUp;
    switch (a) {
        case Direction::kUp : cout << "Going up!\n";
            break;
        case Direction::kDown : cout << "Going down!\n";
            break;  
        case Direction::kLeft : cout << "Going left!\n";
            break;
        case Direction::kRight : cout << "Going right!\n";
            break;
    }
}