# VS Code Configuration

## Style

MSR uses [ROS2 Style Guide](https://docs.ros.org/en/rolling/The-ROS2-Project/Contributing/Code-Style-Language-Versions.html#id1 "https://docs.ros.org/en/rolling/The-ROS2-Project/Contributing/Code-Style-Language-Versions.html#id1") (variation on Google style guide), includes details on variable and function naming.

- Classes: camelCase (CamelCase)
- Methods & Functions: either CamelCase or snake_case, but prefer what the rest of the project is doing

[Linter](https://marketplace.visualstudio.com/items?itemName=mine.cpplint "https://marketplace.visualstudio.com/items?itemName=mine.cpplint") (Uses [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html "https://google.github.io/styleguide/cppguide.html") - can't figure out how to use the ros2 cpplint.py)

- Set linelength=100 to match ROS2 style
- Install per extension instructions (installed path = `/usr/local/bin/cpplint`)
- ROS2 lint path = `/opt/ros/foxy/bin/ament_cpplint` (can't get this to work, [this repo](https://github.com/athackst/vscode-ament-task-provider "https://github.com/athackst/vscode-ament-task-provider") seems like a clue)

## Intellisense

Setup [c_cpp_properties.json](https://code.visualstudio.com/docs/cpp/config-linux#_cc-configurations "https://code.visualstudio.com/docs/cpp/config-linux#_cc-configurations"). This config worked for udacity project which uses g++ and c++17

```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "/usr/local/include/io2d/**",
                "/usr/include/cairo/**",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

## Dependencies

- **TODO**

## Extensions

- C/C++ by Microsoft
- C/C++ Extension Pack by Microsoft
- Clang-Format by Xaver Hallauer (not sure what this is doing)
- CMake by twxs
- CMake Tools by Microsoft
- cpplint by mine

## Other

- **TODO** settings.json
- **TODO** tasks.json

# How To Install Third Party Libraries

## Intellisense

Can add source file path, or install file path, to `c_cpp_properties.json` (sometimes important to add it first so its found first)

```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/0_cpp_foundations/4_route_planner_project/P0267_RefImpl/P0267_RefImpl/P0267_RefImpl/cairo/xlib/**",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

(although that gave an error for udacity project for some nested dependency, but I think would work in other circumstances)

or

```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "/usr/local/include/io2d/**",
                "/usr/include/cairo/**",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

## Build and Configure IDE

Depends on how the developer setup CMake, but in general (this will build binaries in the build folder, which IDE can then point to for includes)

```
mkdir build
cd build
cmake ..
make
```

Might have option to `install` to machine (this will put those same binaries in a common location, which IDE can also point to). There is no rule but usually this installs to /usr/local (i.e., /usr/local/bin for binaries).

```
mkdir build
cd build
cmake ..
make install
```

Can dry-run `make install` steps to see what steps occurred

```
make -n install
```

To uninstall, depends on developer, but in general

```
make uninstall
```

or

```
sudo make uninstall
```

if that fails, can use `make -n install` to dry run and see where binaries were put, then manually undo those steps

# Build/Run/Debug

## Build/Run/Debug: Single File

Open .cpp file and then...

### Build

- Option A: `Ctrl-Shift-P` -> `Run C/C++ File` (this will also run)
- Option B: `Run C/C++ File` in top right corner (this will also run)
- Option C: `g++ <filepath> <executable name>`

### Run

- Option A: `Ctrl-Shift-P` -> `Run C/C++ File` (this will also build)
- Option B: `Run C/C++ File` in top right corner (this will also build)
- Option C: `./<executable path>`

### Debug

- Option A: `Ctrl-Shift-P` -> `Debug C/C++ File`
- Option B: Click `Debug C/C++ File` in top right corner
- Option C: Command line `TBD`

## Build/Run/Debug: Multiple Files

### Build

- Option A: Manual Command line method: `g++ <file1path> <file2path> ... <executable name>` (TODO verify this is correct)
- Option B: VS Code Plugin Method: `Ctrl-Shift-P` -> `Cmake: Release`
- Option C: CMake Command line method:
    
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
    

### Run

- Command line method: `./<executable path>`

### Debug

Add breakpoint, then...

- Option A: `Ctrl-Shift-P` -> `Cmake: Debug`
- Option B: Click bottom of VS Code -> Choose Debug Build -> Click Debug icon -> Click Build
- Option C (allows more configuration such as argument passing): Click `Run and Debug` -> `gdb launch` -> `Create launch.json` and set path to target, such as
    
    ```
    "program": "${workspaceFolder}/build/OSM_A_star_search",
    ```
    
- Option C: Is there a command line for this? [here](https://code.visualstudio.com/docs/cpp/cmake-linux "https://code.visualstudio.com/docs/cpp/cmake-linux").

If not working, consult the VS Code instructions for CMake including Debug [here](https://code.visualstudio.com/docs/cpp/cmake-linux "https://code.visualstudio.com/docs/cpp/cmake-linux").

# Unit Tests

[Google Test Github](https://github.com/google/googletest "https://github.com/google/googletest")

[Google Test Primer](https://google.github.io/googletest/primer.html "https://google.github.io/googletest/primer.html")

[Google Test Example Repo](https://github.com/bmorcos/gtest-example "https://github.com/bmorcos/gtest-example")

## Writing Tests

Write test functions using google's `TEST` macro. Example from google [here](https://google.github.io/googletest/primer.html#simple-tests "https://google.github.io/googletest/primer.html#simple-tests"). Simple example from nanodegree project [here](https://github.com/lotockib/nanodegree-route-planner/blob/google_tests/test/utest_example.cpp "https://github.com/lotockib/nanodegree-route-planner/blob/google_tests/test/utest_example.cpp") below. Note there is no main function here (not sure when that's required vs when not).

tests/utest_example.cpp

```cpp
#include <gtest/gtest.h>
#include <math.h>

int Squared(int n) {
// Returns the factorial of n
    return pow(n, 2);
}; 

// Tests factorial of 0.
TEST(SquaredTest, HandlesZeroInput) {
  EXPECT_EQ(Squared(0), 0);
}

// Tests factorial of positive numbers.
TEST(SquaredTest, HandlesPositiveInput) {
  EXPECT_EQ(Squared(1), 1);
  EXPECT_EQ(Squared(2), 4);
  EXPECT_EQ(Squared(3), 9);
  EXPECT_EQ(Squared(8), 64);
}
```

## Building Tests

One method on multi file projects that use CMake is to add an executable in CMakeLists.txt ([example](https://github.com/lotockib/nanodegree-route-planner/blob/google_tests/CMakeLists.txt "https://github.com/lotockib/nanodegree-route-planner/blob/google_tests/CMakeLists.txt")).

CMakeLists.txt

```cmake
# Isolated simple test for learning google tests
add_executable(simple_test test/utest_simple.cpp)
target_link_libraries(simple_test gtest_main)
```

It will create an executable in the build folder, just like when making an executable for the project itself.

## Running Tests

Same as programs, within build folder, run `./<test_name>`. From the example above:

```shell
cd build
./simple_test
```