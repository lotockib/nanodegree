
# Route Planning

## Build
To get started with your project, you can download the code from the GitHub repo [here](https://github.com/udacity/CppND-Route-Planning-Project), or you can use the workspace provided below. If you decide to work on this project on your local machine, you will need to install the dependencies outlined in the GitHub [README](https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md) for the project, which can be difficult.

In this classroom concept, you will see the instructions for building, running, and testing your project. In the next classroom concept, you will see a detailed overview of the project code, so you'll be well prepared to start work on the project.

### Build and Install Udacity repo and IO2d repo
1. Clone library next to udacity repo
    ```
    git clone git@github.com:udacity/CppND-Route-Planning-Project.git
    cd CppND-Route-Planning-Project.git
    git submodule update --init --recursive
    ```
2. Follow build steps below, modified from [here](https://github.com/cpp-io2d/P0267_RefImpl/blob/master/BUILDING.md)

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
    sudo make install (this installs library to machine)
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