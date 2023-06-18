# Project

This directory is a manual copy of [CppND-Route-Planning-Project](https://github.com/udacity/CppND-Route-Planning-Project) to allow for editing and project submission.

# Setup

1. Install dependencies per [Udacity repo instructions](https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md#dependencies-for-running-locally), except for building io2d, follow these modified steps to avoid errors
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
    mkdir build
    cd build
    cmake --config Release "-DCMAKE_BUILD_TYPE=Release" ..
    make
    sudo make install
    ```

2. Clone this repo and initialize submodules.

    ```
    git clone https://github.com/lotockib/nanodegree.git --recurse-submodules
    ```
3. Build.

    ```
    cd nanodegree/0_cpp_foundations/4_route_planner_project/CppND-Route-Planning-ProjectCopy/
    mkdir build
    cd build
    cmake ..
    make
    ```

# Test

```
cd build
./test
```

# Run

```
cd build
./OSM_A_star_search
```

<img src="map.png" width="600" height="450" />