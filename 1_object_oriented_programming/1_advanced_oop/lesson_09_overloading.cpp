#include <iostream>

class Human {};
class Dog {};
class Cat {};

// TODO: Write hello() function
void Hello() { std::cout << "Hello\n"; }

// TODO: Overload hello() three times
void Hello(Human h) { std::cout << "Hello, human\n"; }
void Hello (Dog d) { std::cout << "Hello, dog\n"; }
void Hello (Cat d) { std::cout << "Hello, cat\n"; }

// TODO: Call hello() from main()
int main() {
    Hello();
    Hello(Human());
    Hello(Dog());
    Hello(Cat());
}