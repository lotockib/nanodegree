#include <iostream>
using std::cout;

int main()
{
    // if statement
    bool a = true;
    if (a) {
        cout << "A is true" << "\n";
    }

    // while loop
    cout << "Print 0 through 4:\n";
    auto i = 0;
    while (i < 5) {
        cout << i << "\n";
        i++;
    }

    // practice
    cout << "Print even numbers only:\n";
    auto j = 1;
    while (j <= 10) {
        if ( j % 2 == 0 )
            cout << j << "\n";
        j++;
    }

}