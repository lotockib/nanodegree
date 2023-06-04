// auto

#include <iostream>
#include <vector>

using std::cout;
using std::vector;

int main() {
    auto a = 6;
    auto b = {1, 2, 3};
    cout << "variables were declared with auto\n";

    auto c = {7, 8, 9, 10};
    for (auto i : c)
        cout << i << " ";
    cout << "\n";

}