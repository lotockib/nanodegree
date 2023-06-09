#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using std::cout;
using std::ifstream;
using std::string;
using std::vector;
using std::istringstream;

// TODO: Add the ParseLine function here.
vector<int> ParseLine(string input_string) {
    istringstream mystream(input_string);
    int i;
    char c;
    vector<int> v {};
    while (mystream >> i >> c) {
        v.push_back(i);
    }
    return v;
}

void ReadBoardFile(string path) {
  ifstream myfile (path);
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      cout << line << "\n";
    }
  }
}

void PrintBoard(const vector<vector<int>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << board[i][j];
    }
    cout << "\n";
  }
}

#include "./lesson_24_test.cpp" // For testing.

int main() {
  ReadBoardFile("files/1.board");
  TestParseLine(); // For testing.
  // Leave commented out.
  // PrintBoard(board);
}