// vector access

#include <iostream>
#include <vector>
using std::cout;
using std::vector;

int main() {
    
    // 1d init
    vector<int> a = {0, 1, 2, 3, 4};
    
    // 1d print manually
    cout << a[0] << " ";
    cout << a[1] << " ";
    cout << a[2] << " ";
    cout << a[3] << " ";
    cout << a[4] << " ";
    cout << "\n";

    // 1d print using for loop
    for (int i : a)
        cout << i << " ";
    cout << "\n";

    // 2d init
    vector<vector<int>> b =     {{1, 1, 2, 3},
                                 {2, 1, 2, 3},
                                 {3, 1, 2, 3}};

    // 2d print manually
    cout << "b00 = " << b[0][0] << "\n";

    // 2d print using for loop
    for (auto i : b) {
        for (auto j : i)
            cout << j << " ";
        cout << "\n";
    }
}