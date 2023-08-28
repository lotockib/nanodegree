// Processing strings using istreamstream

#include <iostream>
#include <sstream>
#include <string>
using std::string;
using std::istringstream;
using std::cout;

int main() {
    
    // Basic example
    string a {"199 2 3"};
    istringstream mystream(a);
    int b;
    mystream >> b; // shifts until there is whitespace or some other interruption
    cout << b << "\n";

    // Use istringstream to test if stream was successful
    string c {"199 2 3"};
    istringstream mystream2(c);
    int d;
    while (mystream2) { // Checks istringstream to see if it's valid
        mystream2 >> d;
        if (mystream2) { // Checks istringstream for valid shift on previous line
            cout << "Successful stream read: " << d << "\n";
        }
        else {
            cout << "Failed stream\n";
        }
    }

    // More concise to use operation in the while loop evaluator
    string e {"199 2 3"};
    istringstream mystream3(e);
    int f;
    while (mystream3 >> f) {
        cout << "Successful stream read: " << f << "\n";
    }
    cout << "Failed stream\n";

    // Mixed types
    string g {"199,2,3,"};
    istringstream mystream4(g);
    int h;
    char i;
    while (mystream4 >> h >> i) {
        cout << "Successful stream read:" << h << i << std::endl;
    }
    cout << "Failed stream" << std::endl;
}