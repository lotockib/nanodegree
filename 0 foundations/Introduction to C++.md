## compiling

C++ is a compiled programming language, which means that programmers use a program to compile their human-readable source code into machine-readable object and executable files. The program that performs this task is called a compiler.

C++ does not have an "official" compiler. Instead, there are [many different compilers](https://www.stroustrup.com/compilers.html "https://www.stroustrup.com/compilers.html") that a programmer can use.

GNU Compiler Collection is a popular, open-source, cross-platform compiler from the larger GNU Project. In particular, we use the g++ program, which is a command line executable that compiles C++ source code and automatically [links](https://en.wikipedia.org/wiki/Linker_(computing) "https://en.wikipedia.org/wiki/Linker_(computing)") the C++ Standard Library.

In order to use classes and functions from the C++ Standard Library, the compiler must have access to a compiled version of the standard library, stored in object files. Most compiler implementations, including GCC, include those object files as part of the installation process. In order to use the Standard Library facilities, the compiler must ["link"](https://en.wikipedia.org/wiki/Linker_(computing) "https://en.wikipedia.org/wiki/Linker_(computing)") the standard library object files to the object files created from the programmer's source code.

Once the compiler links together the necessary object files, it is able to generate a standalone executable file that can run on the operating system.
![[0 foundations/code/1_introduction_to_cpp/c-compilation-process.svg]]

## commenting

Find the balance: don't comment every line because the code should be somewhat readable, but include some comments on the intent and what it 'should do' (possibly in blocks)

[good article on commenting](https://visualstudiomagazine.com/Kunk0211 "https://visualstudiomagazine.com/Kunk0211")

[another article](https://visualstudiomagazine.com/Kunk0211 "https://visualstudiomagazine.com/Kunk0211")

[clean code by Robert Martin](https://ptgmedia.pearsoncmg.com/images/9780132350884/samplepages/9780132350884.pdf "https://ptgmedia.pearsoncmg.com/images/9780132350884/samplepages/9780132350884.pdf")

## cout

```
#include <iostream>
using std::cout;
cout << "Hello!" << "\n";
```

## primitives

Built-in data types that can be used to declare variables.

Integer Character Boolean Floating Point Double Floating Point Valueless or Void Wide Character

[good explanation of data types](https://www.geeksforgeeks.org/cpp-data-types/ "https://www.geeksforgeeks.org/cpp-data-types/")

[cpp reference definitions](https://en.cppreference.com/w/cpp/language/types "https://en.cppreference.com/w/cpp/language/types")

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

Makes refactoring more efficient. [More info about auto](https://www.geeksforgeeks.org/type-inference-in-c-auto-and-decltype/ "https://www.geeksforgeeks.org/type-inference-in-c-auto-and-decltype/")

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

## functions

```
int AdditionFunctionv1(vector<int> vec) {
    int sum = 0;
    for (int i : vec) {
        sum += i;
    }
    return sum;
}
```

## if and while loops

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

Put string into a stream object. Then shift that objects into a varible.

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

Using the `class` keyword means it is a scoped Enum. You can removed 'class' for unscoped Enum.

```
enum class Color {white, black, red, blue};
```