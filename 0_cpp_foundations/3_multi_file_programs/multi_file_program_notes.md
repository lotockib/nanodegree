# Multi File Programs

## Header Files
- Declarations can be moved to a separate file called a header file.
- Header files are good for
  - Solving the problem of having functions in the right order: you can't call a function unless it's previously been defined.
  - Re using functions across multiple files: you don't want to duplicate code.
  - Organizing code: Hiding function definitions in other files can make for cleaner main.
- Example
  - header_example.cpp
    ```
    // The header file with just the function declarations.
    // When you click the "Run Code" button, this file will
    // be saved as header_example.h.
    #ifndef HEADER_EXAMPLE_H
    #define HEADER_EXAMPLE_H

    void OuterFunction(int);
    void InnerFunction(int);

    #endif
    ```
  - main.cpp
    ```
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
  - Details
    - The function declarations in the header file don't need variable names, just variable types. You can put names in the declaration, however, and doing this often makes the code easier to read.
    - The #include statement for the header used quotes " " around the file name, and not angle brackets <>. We have stored the header in the same directory as the .cpp file, and the quotes tell the preprocessor to look for the file in the same directory as the current file - not in the usual set of directories where libraries are typically stored.
    - Finally, there is a preprocessor directive:
    #ifndef HEADER_EXAMPLE_H
    #define HEADER_EXAMPLE_H
    at the top of the header, along with an #endif at the end. This is called an "include guard". Since the header will be included into another file, and #include just pastes contents into a file, the include guard prevents the same file from being pasted multiple times into another file. This might happen if multiple files include the same header, and then are all included into the same main.cpp, for example. The ifndef checks if HEADER_EXAMPLE_H has not been defined in the file already. If it has not been defined yet, then it is defined with #define HEADER_EXAMPLE_H, and the rest of the header is used. If HEADER_EXAMPLE_H has already been defined, then the preprocessor does not enter the ifndef block. Note: There are other ways to do this. Another common way is to use an #pragma oncepreprocessor directive, but we won't cover that in detail here. See this Wikipedia article for examples.
- Gotchas
  - don't forget include guards so header doesn't get copied multiple times
  - don't forget to include libraries (ie ```#include <vector>``` in the header too)
  - when compiling, you must add each file as an argument
  ```g++ -std=c++17 ./code/main.cpp ./code/increment_and_sum.cpp ./code/vect_add_one.cpp && ./a.out```
When compiling, each of the relevant .cpp files must be included in the compile command. The -std=c++17 specifies that we are using the C++ 17 standard (which happens automatically in the terminal).

## CMake and Make

### Object Files
Object Files
When you compile a project with g++, g++ actually performs several distinct tasks:

The preprocessor runs and executes any statement beginning with a hash symbol: #, such as #include statements. This ensures all code is in the correct location and ready to compile.
Each file in the source code is compiled into an "object file" (a .o file). Object files are platform-specific machine code that will be used to create an executable.

The object files are "linked" together to make a single executable. In the examples you have seen so far, this executable is a.out, but you can specify whatever name you want.
It is possible to have g++ perform each of the steps separately by using the -c flag. For example,


```g++ -c main.cpp```

will produce a main.o file, and that file can be converted to an executable with

```g++ main.o```