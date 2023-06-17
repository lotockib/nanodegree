# Project

This directory is a manual copy of [CppND-Route-Planning-Project](https://github.com/udacity/CppND-Route-Planning-Project) to allow for editing and project submission.

# Setup

1. Install dependencies per [Udacity repo instructions](https://github.com/udacity/CppND-Route-Planning-Project/blob/master/README.md#dependencies-for-running-locally).

1. Clone this repo and initialize submodules.

    ```
    git clone https://github.com/lotockib/nanodegree.git --recurse-submodules
    ```
2. Build.

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