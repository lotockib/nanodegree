Popular search algorithm for graphs.

## Motion Planning Overview

- Discrete method: chop motion into small plans
- Givens
    - Map
    - Starting location
    - Goal location
    - Cost
- Goal
    - Find minimum cost path

## Procedures

- General procedure
    - From start, identify valid steps
    - For each step, identify number of expansions that have occurred
    - For next step, always choose one with leave number of expansions (g value)
- A* procedure
    - Uses heuristic function ie "h(x,y) <= distance to goal from x0,y0", aka h value (see [code](https://file+.vscode-resource.vscode-cdn.net/home/blotocki/git/nanodegree/0_cpp_foundations/2_astar_search/lesson_09_heuristic.cpp "./lesson_09_heuristic.cpp"))
    - f value = g + h
    - Still use open list, g-value, but also record g-value + heuristic value (f value is sum)
    - Each step: g-value has information about distance already taken, heuristic value has information about distance remaining

## Pseudocode

Search( grid, initial_point, goal_point ) :

1. Initialize an empty list of open nodes.
2. Initialize a starting node with the following:
    - x and y values given by initial_point.
    - g = 0, where g is the cost for each move.
    - h given by the heuristic function (a function of the current coordinates and the goal).
3. Add the new node to the list of open nodes.
4. while the list of open nodes is nonempty:
    - Sort the open list by f-value
    - Pop the optimal cell (called the current cell).
    - Mark the cell's coordinates in the grid as part of the path.
    - if the current cell is the goal cell:
        - return the grid.
    - else, expand the search to the current node's neighbors. This includes the following steps:
        - Check each neighbor cell in the grid to ensure that the cell is empty: it hasn't been closed and is not an obstacle.
            - If the cell is empty, compute the cost (g value) and the heuristic, and add to the list of open nodes.
            - Mark the cell as closed.
    - If you exit the while loop because the list of open nodes is empty, you have run out of new nodes to explore and haven't found a path.

## Structure
![[Pasted image 20230827205326.png]]
## C++ Features

### const vs constexpr

This example highlights how to use const to promise not to modify a variable, even though the variable can only be evaluated at run time.

The example also show how to use constexpr to guarantee that a variable can be evaluated at compile time.

### arrays

Use arrays when length doesn't need to change, they are slightly more efficient than vectors. Array represents a fixed chunk of memory.

### references

In function argument, use ampersand to say it's a reference to the variable and not a copy of it.

# How to get good

Learn standard libraries. Understand the concepts, don't just copy/past from the web.