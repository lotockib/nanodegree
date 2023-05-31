// vectors

#include <iostream>
#include <vector>
using std::vector;
using std::cout;

int main() {
    // three ways of declaring
    vector<int> v_1{0, 1, 2}; // preferred
    vector<int> v_2 = {3, 4, 5};
    vector<int> v_3;
    v_3 = {6};
    cout << "Everything worked" << "\n";

    // 2d vector
    vector<vector<int>> v_4 {{1,2}, {3,4}};
    cout << "A 2d vector has been created." << "\n";

    // practice
    vector<int> v_5{6, 7, 8};
    cout << "v_5 has been created.\n";
    for (int i : v_5)
        cout << i << " ";
    cout << "\n";

}