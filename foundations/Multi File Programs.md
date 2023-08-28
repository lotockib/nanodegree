## Header Files

Declarations can be moved to a separate file called a header file. Implementations can exist in a file that `#include` that header file.

Header files are good for

- Solving the problem of having functions in the right order: you can't call a function unless it's previously been defined.
- Re using functions across multiple files: you don't want to duplicate code.
- Organizing code: Hiding function definitions in other files can make for cleaner main.

header_example.h

```cpp
// The header file with just the function declarations.
// When you click the "Run Code" button, this file will
// be saved as header_example.h.
#ifndef HEADER_EXAMPLE_H
#define HEADER_EXAMPLE_H

void OuterFunction(int);
void InnerFunction(int);

#endif
```

main.cpp

```cpp
// The contents of header_example.h are included in 
// the corresponding .cpp file using quotes:
#include "header_example.h"

#include <iostream>
using std::cout;

void OuterFunction(int i) 
{
    InnerFunction(i);
}

void InnerFunction(int i) 
{
    cout << "The value of the integer is: " << i << "\n";
}

int main() 
{
    int a = 5;
    OuterFunction(a);
}
```

Details

- The function declarations in the header file don't need variable names, just variable types. You can put names in the declaration, however, and doing this often makes the code easier to read.
- The #include statement for the header used quotes " " around the file name, and not angle brackets <>. We have stored the header in the same directory as the .cpp file, and the quotes tell the preprocessor to look for the file in the same directory as the current file - not in the usual set of directories where libraries are typically stored.
- Finally, there is a preprocessor directive: #ifndef HEADER_EXAMPLE_H #define HEADER_EXAMPLE_H at the top of the header, along with an #endif at the end. This is called an "include guard". Since the header will be included into another file, and #include just pastes contents into a file, the include guard prevents the same file from being pasted multiple times into another file. This might happen if multiple files include the same header, and then are all included into the same main.cpp, for example. The ifndef checks if HEADER_EXAMPLE_H has not been defined in the file already. If it has not been defined yet, then it is defined with #define HEADER_EXAMPLE_H, and the rest of the header is used. If HEADER_EXAMPLE_H has already been defined, then the preprocessor does not enter the ifndef block. Note: There are other ways to do this. Another common way is to use an #pragma oncepreprocessor directive, but we won't cover that in detail here. See this Wikipedia article for examples.
- Gotchas
    - don't forget include guards so header doesn't get copied multiple times
    - don't forget to include libraries (ie `#include <vector>` in the header too)
    - when compiling, you must add each file as an argument `g++ -std=c++17 ./code/main.cpp ./code/increment_and_sum.cpp ./code/vect_add_one.cpp && ./a.out` When compiling, each of the relevant .cpp files must be included in the compile command. The -std=c++17 specifies that we are using the C++ 17 standard (which happens automatically in the terminal).

[Example](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/0_cpp_foundations/3_multi_file_programs/multi_file_learning/main.cpp "./multi_file_learning/main.cpp") of program using header file, implementation file, and main file.

## CMake and Make

### Object Files

Object Files When you compile a project with g++, g++ actually performs several distinct tasks:

The preprocessor runs and executes any statement beginning with a hash symbol: #, such as #include statements. This ensures all code is in the correct location and ready to compile. Each file in the source code is compiled into an "object file" (a .o file). Object files are platform-specific machine code that will be used to create an executable.

The object files are "linked" together to make a single executable. In the examples you have seen so far, this executable is a.out, but you can specify whatever name you want. It is possible to have g++ perform each of the steps separately by using the -c flag. For example,

`g++ -c main.cpp`

will produce a main.o file, and that file can be converted to an executable with

`g++ main.o`

### CMake

- CMakeLists.txt can live in every directory
- Please note that CMake isn't a build system. It's a build system generator. This is why we need to invoke make after running CMake. Running CMake generates Makefiles with the appropriate platform dependencies, and running make uses them.
- [VS Code Instructions for setting up CMake](https://code.visualstudio.com/docs/cpp/cmake-linux "https://code.visualstudio.com/docs/cpp/cmake-linux")
- FYI of [how to write makefiles](https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html "https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html") (don't need to actually do this, Cmake does it for us)
- Inside:
    - minimum cmake version specified
    - sent environmental variable
    - name project
    - list of executables

### References

Example

```
#include <iostream>
using std::cout;

int main() 
{
    int i = 1;
    
    // Declare a reference to i.
    int& j = i;
    cout << "The value of j is: " << j << "\n";
    
    // Change the value of i.
    i = 5;
    cout << "The value of i is changed to: " << i << "\n";
    cout << "The value of j is now: " << j << "\n";
    
    // Change the value of the reference.
    // Since reference is just another name for the variable,
    // th
    j = 7;
    cout << "The value of j is now: " << j << "\n";
    cout << "The value of i is changed to: " << i << "\n";
}
```

### Pointers

Example of accessing memory address

```
#include <iostream>
using std::cout;

int main() {
    int i = 5;
    int j = 6;
    
    // Print the memory addresses of i and j
    cout << "The address of i is: " << &i << "\n";
    cout << "The address of j is: " << &j << "\n";
}
```

Example creating pointer

```
#include <iostream>
using std::cout;

int main() 
{
    int i = 5;
    // A pointer pointer_to_i is declared and initialized to the address of i.
    int* pointer_to_i = &i;
    
    // Print the memory addresses of i and j
    cout << "The address of i is:          " << &i << "\n";
    cout << "The variable pointer_to_i is: " << pointer_to_i << "\n";
}
```

Example getting value back from pointer

```
#include <iostream>
using std::cout;

int main() 
{
    int i = 5;
    // A pointer pointer_to_i is declared and initialized to the address of i.
    int* pointer_to_i = &i;
    
    // Print the memory addresses of i and j
    cout << "The address of i is:          " << &i << "\n";
    cout << "The variable pointer_to_i is: " << pointer_to_i << "\n";
    cout << "The value of the variable pointed to by pointer_to_i is: " << *pointer_to_i << "\n";
}
```

The symbols & and * have a different meaning, depending on which side of an equation they appear.

This is extremely important to remember. For the & symbol, if it appears on the left side of an equation (e.g. when declaring a variable), it means that the variable is declared as a reference. If the & appears on the right side of an equation, or before a previously defined variable, it is used to return a memory address, as in the example above.

Exampe using pointer in function

```
#include <iostream>
using std::cout;

void AddOne(int* j)
{
    // Dereference the pointer and increment the int being pointed to.
    (*j)++;
}

int main() 
{
    int i = 1;
    cout << "The value of i is: " << i << "\n";
    
    // Declare a pointer to i:
    int* pi = &i;
    AddOne(pi);
    cout << "The value of i is now: " << i << "\n";
}
```

When using pointers with functions, some care should be taken. If a pointer is passed to a function and then assigned to a variable in the function that goes out of scope after the function finishes executing, then the pointer will have undefined behavior at that point - the memory it is pointing to might be overwritten by other parts of the program.

Example: return pointer

```
#include <iostream>
using std::cout;

int* AddOne(int& j) 
{
    // Increment the referenced int and return the
    // address of j.
    j++;
    return &j;
}

int main() 
{
    int i = 1;
    cout << "The value of i is: " << i << "\n";
    
    // Declare a pointer and initialize to the value
    // returned by AddOne:
    int* my_pointer = AddOne(i);
    cout << "The value of i is now: " << i << "\n";
    cout << "The value of the int pointed to by my_pointer is: " << *my_pointer << "\n";
}
```

### References vs Pointers

References are generally easier and safer than pointers. As a decent rule of thumb, references should be used in place of pointers when possible.

However, there are times when it is not possible to use references. One example is object initialization. You might like one object to store a reference to another object. However, if the other object is not yet available when the first object is created, then the first object will need to use a pointer, not a reference, since a reference cannot be null. The reference could only be initialized once the other object is created.

### Maps

A map (alternatively hash table, hash map, or dictionary) is a data structure that uses key/value pairs to store data, and provides efficient lookup and insertion of the data.

Example

```
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using std::vector;
using std::cout;
using std::unordered_map;
using std::string;


int main() {
    // Create strings to use in the hash table.
    string key = "word";
    string def_1 = "a unit of language, consisting of one or more spoken sounds or their written representation, that functions as a principal carrier of meaning";
    string def_2 = "speech or talk: to express one's emotion in words";
    string def_3 = "a short talk or conversation: 'Marston, I'd like a word with you.'";
    string def_4 = "an expression or utterance: a word of warning";
    unordered_map <string, vector<string>> my_dictionary;

    // Check if key is in the hash table.
    if (my_dictionary.find(key) == my_dictionary.end()) {
        cout << "The key 'word' is not in the dictionary." << "\n";
        cout << "Inserting a key-value pair into the dictionary." << "\n\n";
        // Set the value for the key.
        my_dictionary[key] = vector<string> {def_1, def_2, def_3, def_4};
    }

    // The key should now be in the hash table. You can access the
    // value corresponding to the key with square brackets [].
    // Here, the value my_dictionary[key] is a vector of strings.
    // We iterate over the vector and print the strings.
    cout << key << ": \n";
    auto definitions = my_dictionary[key];
    for (string definition : definitions) {
        cout << definition << "\n";
    }
}
```

### OOP

OOP is a style of coding that collects related data (object attributes) and functions (object methods) together to form a single data structure, called an object. This allows that collection of attributes and methods to be used repeatedly in your program without code repetition.

In C++ the attributes and methods that make up an object are specified in a code class, and each object in the program is an instance of that class.

Example

```
#include <iostream>
#include <string>
using std::string;
using std::cout;

class Car {
  public:
    void PrintCarData() 
    {
        cout << "The distance that the " << color << " car " << number << " has traveled is: " << distance << "\n";
    }

    void IncrementDistance() 
    {
        distance++;
    }
    
    // Adding a constructor here:
    Car(string c, int n) 
    {
        // Setting the class attributes with
        // The values passed into the constructor.
        color = c;
        number = n;
    }
    
    string color;
    int distance = 0;
    int number;
};

int main() 
{
    // Create class instances for each car.
    Car car_1 = Car("green", 1);
    Car car_2 = Car("red", 2);
    Car car_3 = Car("blue", 3);

    // Increment car_1's position by 1.
    car_1.IncrementDistance();

    // Print out the position and color of each car.
    car_1.PrintCarData();
    car_2.PrintCarData();
    car_3.PrintCarData();
}
```

### Classes In Multiple Files

car.h: initialize with list instead of normal way. Private means main function can't access the private data.

```
#ifndef CAR_H
#define CAR_H

#include <string>
using std::string;
using std::cout;

class Car {
  public:
    void PrintCarData();
    void IncrementDistance();
    
    // Using a constructor list in the constructor:
    Car(string c, int n) : color(c), number(n) {}
  
  // The variables do not need to be accessed outside of
  // functions from this class, so we can set them to private.
  private:
    string color;
    int distance = 0;
    int number;
};

#endif
```

car.pp: Defines functions, must use `Car::` to tell it which class the function refers to.

```
#include <iostream>
#include "car.h"

// Method definitions for the Car class.
void Car::PrintCarData() 
{
    cout << "The distance that the " << color << " car " << number << " has traveled is: " << distance << "\n";
}

void Car::IncrementDistance() 
{
    distance++;
}
```

car_main.cpp: just include car.h, nothing else special

```
#include <iostream>
#include <string>
#include "car.h"
using std::string;
using std::cout;

int main() 
{
    // Create class instances for each car.
    Car car_1 = Car("green", 1);
    Car car_2 = Car("red", 2);
    Car car_3 = Car("blue", 3);

    // Increment car_1's position by 1.
    car_1.IncrementDistance();

    // Print out the position and color of each car.
    car_1.PrintCarData();
    car_2.PrintCarData();
    car_3.PrintCarData();

}
```

The arrow operator -> is used to simultaneously

dereference a pointer to an object and access an attribute or method.

For example, in the code below, cp is a pointer to a Car object, and the following two are equivalent:

```
// Simultaneously dereference the pointer and 
// access IncrementDistance().
cp->IncrementDistance();

// Dereference the pointer using *, then 
// access IncrementDistance() with traditional 
// dot notation.
(*cp).IncrementDistance();
```

### This Pointer

It is possible to make this explicit in C++ by using the this pointer, which points to the current class instance. Using this can sometimes be helpful to add clarity to more complicated code:

Example without this pointer

```
// The Car class
class Car {
  public:
    // Method to print data.
    void PrintCarData() {
        cout << "The distance that the " << color << " car " << number << " has traveled is: " << distance << "\n";
    }
    
    // Method to increment the distance travelled.
    void IncrementDistance() {
        distance++;
    }
    
    // Class/object attributes
    string color;
    int distance = 0;
    int number;
};
```

Example with this pointer

```
// The Car class
class Car {
  public:
    // Method to print data.
    void PrintCarData() {
        cout << "The distance that the " << this->color << " car " << this->number << " has traveled is: " << this->distance << "\n";
    }
    
    // Method to increment the distance travelled.
    void IncrementDistance() {
        this->distance++;
    }
    
    // Class/object attributes
    string color;
    int distance = 0;
    int number;
};
```