#include <iostream>
using std::cout;

// ampersand means another name for original variable is being passed, instead of copied
int MultiplyByTwo(int &i) {
    i = 2*i;
    return i;
}

int main() {
    int a = 5;
    cout << "The int a equals: " << a << "\n";
    int b = MultiplyByTwo(a);
    cout << "The int b equals: " << b << "\n";
    cout << "The int a now equals: " << a << "\n";
}