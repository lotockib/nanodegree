// for loops

#include <iostream>
#include <vector>
using std::vector;
using std::cout;

int main() {

    // manual indexing
    for (int i=0; i<5; i++) {
        cout << i << "\n";
    }

    // post increment
    auto i = 1;
    auto c = i++; // return happens first, then increment second, so c = 1
    cout << "post increment\n";
    cout << "i = " << i << "\n";
    cout << "c = " << c << "\n";

    // pre increment
    i = 1; // reset i
    c = ++i; // increment happens first, then return second, so c = 2
    cout << "pre increment\n";
    cout << "i = " << i << "\n";
    cout << "c = " << c << "\n";

    // decrement
    i--;
    cout << "decrement\n";
    cout << "i = " << i << "\n";

    // practice
    cout << "print -3 through 10" << "\n";
    for (int i = -3; i < 11; i++)
        cout << i << " ";
    cout << "\n";

    // range based for loop
    cout << "range based for loop\n";
    vector<int> d = {1, 2, 3, 4, 5};
    for (int j : d)
        cout << j << " ";
    cout << "\n";

    // challenge
    cout << "challenge: print 2d vector using range based for loop\n";
    vector<vector<int>> e =     {{1, 2},
                                 {3, 4},
                                 {5, 6}};
    for (vector<int> k : e) {
        for (int l : k)
            cout << l << " ";
        cout << "\n";
    }
}