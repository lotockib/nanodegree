#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using std::cout;
using std::string;
using std::vector;

// TODO: Add the ReadBoardFile function here.
void ReadBoard(const string path) {
    std::ifstream myfile;
    myfile.open(path);
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
                cout << line << "\n";
            }
    }
}

// PrintBoard not used in this exercise
void PrintBoard(const vector<vector<int>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << board[i][j];
    }
    cout << "\n";
  }
}

int main() {
  // TODO: Call the ReadBoardFile function here.
  string path = "files/1.board";

  ReadBoard(path);

  
  // Leave the following line commented out.
  //PrintBoard(board);
}