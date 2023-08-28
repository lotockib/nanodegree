## Build

To get started with your project, you can download the code from the GitHub repo [here](https://github.com/udacity/CppND-Route-Planning-Project "https://github.com/udacity/CppND-Route-Planning-Project"), or you can use the workspace provided below. If you decide to work on this project on your local machine, you will need to install the dependencies outlined in the GitHub [README](https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md "https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md") for the project, which can be difficult.

In this classroom concept, you will see the instructions for building, running, and testing your project. In the next classroom concept, you will see a detailed overview of the project code, so you'll be well prepared to start work on the project.

### Build and Install Udacity repo and IO2d repo

1. Clone library next to udacity repo
    
    ```
    git clone git@github.com:udacity/CppND-Route-Planning-Project.git
    cd CppND-Route-Planning-Project.git
    git submodule update --init --recursive
    ```
    
2. Follow build steps below, modified from [here](https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md "https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md")
    
    ```
    sudo apt update
    sudo apt install build-essential
    sudo apt install cmake
    sudo apt install libcairo2-dev
    sudo apt install libgraphicsmagick1-dev
    sudo apt install libpng-dev
    
    git submodule add git@github.com:cpp-io2d/P0267_RefImpl.git
    cd P0267_RefImpl
    git submodule update --init --recursive
    code CMakeListst.txt -> comment out last two sections on TESTS and SAMPLES (why?)
    mkdir build (why not Debug?)
    cd build
    cmake --config Release "-DCMAKE_BUILD_TYPE=Release" ..
    make
    sudo make install (this installs library to machine at ```/usr/local/include/io2d```, can use ```find -name``` to figure it out)
    ```
    
3. Build udacity project
    
    ```
    cd CppND-Route-Planning-Project
    mkdir build
    cd build
    cmake ..
    make
    ```
    
4. Run (should only open map, will do nothing else)
    
    ```
    ./OSM_A_star_search
    ```
    

## Test

After build run tests....

```
cd build
./test
```

Should get output similar to this (errors because code isn't built)

```
[06:48 PM][master]build $ ./test 
Running main() from /home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/thirdparty/googletest/googletest/src/gtest_main.cc
[==========] Running 4 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 4 tests from RoutePlannerTest
[ RUN      ] RoutePlannerTest.TestCalculateHValue
/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/test/utest_rp_a_star_search.cpp:66: Failure
Expected equality of these values:
  route_planner.CalculateHValue(start_node)
    Which is: 0.031656995
  1.1329799
/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/test/utest_rp_a_star_search.cpp:67: Failure
Expected equality of these values:
  route_planner.CalculateHValue(end_node)
    Which is: 4.6308042e+27
  0.0f
    Which is: 0
/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/test/utest_rp_a_star_search.cpp:68: Failure
Expected equality of these values:
  route_planner.CalculateHValue(mid_node)
    Which is: 4.0504058e-12
  0.58903033
[  FAILED  ] RoutePlannerTest.TestCalculateHValue (60 ms)
[ RUN      ] RoutePlannerTest.TestAddNeighbors
/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/test/utest_rp_a_star_search.cpp:84: Failure
Expected equality of these values:
  neighbors.size()
    Which is: 0
  4
[  FAILED  ] RoutePlannerTest.TestAddNeighbors (34 ms)
[ RUN      ] RoutePlannerTest.TestConstructFinalPath
/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-Project/test/utest_rp_a_star_search.cpp:104: Failure
Expected equality of these values:
  path.size()
    Which is: 0
  3
Segmentation fault (core dumped)
```

## Code Architecture

![[Pasted image 20230827205210.png]]
## VS Code Configuration

VS Code intellisense must be told where the includes are for external libraries, compiler path, and c++ version.

For this project, I used these settings for intellisense to work:

c_cpp_properties.json

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

## Extensibility

Example of extensibility is creating a child class that inherits from parent, and adding data to it.

## Project

See [README](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-ProjectCopy/README.md "CppND-Route-Planning-ProjectCopy/README.md").