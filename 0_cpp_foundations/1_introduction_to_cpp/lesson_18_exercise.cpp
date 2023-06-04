// exercise: store grid

#include <iostream>
#include <vector>
using std::vector;
using std::cout;

void PrintBoard(const vector<vector<int>> &board) {
    for (auto row : board) {
        for (auto element : row)
            cout << element;
        cout << "\n";
    }
    return;
}

int main() {
    vector<vector<int>> board;
    board = {{0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0}};

    PrintBoard(board);
}