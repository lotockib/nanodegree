# Introduction to CPP

## cout
```
#include <iostream>
using std::cout;
cout << "Hello!" << "\n";
```
## primitives
What does primitives mean?
```
int i = 1
```
## vectors
Efficient way to store a list of variables.
```
#include <vector>
using std::vector;
vector<int> v_1{0, 1, 2}; // preferred
vector<int> v_2 = {3, 4, 5};
vector<int> v_3;
```
## auto
Makes refactoring more efficient.
```
auto i = 5;
```
## for loops
Range based.
```
int AdditionFunctionv1(vector<int> vec) {
    int sum = 0;
    for (int i : vec) {
        sum += i;
    }
    return sum;
}
```
Iterator based.
```
int AdditionFunctionv4(const vector<int> &v) {
    int sum = 0;
    for(auto i = v.begin(); i != v.end(); ++i)
        sum += *i;
    return sum;
}
```
Index based.
```
int AdditionFunctionv3(const vector<int> &v)
{
    int sum = 0;
    // iterator is not an int, so can use auto
    for(auto i = v.begin(); i!= v.end(); ++i)
        sum += *i; // TODO why does i need to be dereferenced?  it throws error without it
    return sum;
}
```

functions
```
int AdditionFunctionv1(vector<int> vec) {
    int sum = 0;
    for (int i : vec) {
        sum += i;
    }
    return sum;
}
```

if and while loops
```
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
```
## processing strings
Put string into a stream object.  Then shift that objects into a varible.

```
#include <sstream>
#include <string>
using std::string;
using std::istringstream;

string a {"199 2 3"};
istringstream mystream(a);
int b;

mystream >> b; // shifts until there is whitespace or some other interruption
cout << b << "\n";
```
## enums
Using the `class` keyword means it is a scoped Enum.  You can removed 'class' for unscoped Enum.
```
enum class Color {white, black, red, blue};
```