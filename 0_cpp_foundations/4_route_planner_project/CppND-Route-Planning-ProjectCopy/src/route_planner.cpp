#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.
float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    
    // Find distance from current node to end node
    return node->distance(*end_node);

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    // Find neighbors of current node and populate vector
    current_node->FindNeighbors();

    // For each neightbor node
    for (auto neighbor : current_node->neighbors) {

        // Don't process if already visited
        if (neighbor->visited != true) {

            // Set parent to current
            neighbor->parent = current_node;

            // Find distance between current node and this neighbor
            neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);

            // Find h value
            neighbor->h_value = RoutePlanner::CalculateHValue(neighbor);

            // Add to the open list
            open_list.push_back(neighbor);

            // Mark visited
            neighbor->visited = true;
        }

    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

bool Compare(RouteModel::Node * node0, RouteModel::Node * node1) {
    float cost0 = node0->g_value + node0->h_value;
    float cost1 = node1->g_value + node1->h_value;
    return cost0 > cost1;
}

RouteModel::Node *RoutePlanner::NextNode() {

    RouteModel::Node * next_node = nullptr;

    // Sort open_list
    sort(open_list.begin(), open_list.end(), Compare);

    // int i = 0;
    // for (auto open_node : open_list) {
    //     float cost = open_node->g_value + open_node->h_value;
    //     std::cout << "open node " << i << "/" << open_list.size() << " : cost = " << cost << std::endl;
    //     i++;
    // }

    // Get lowest cost node for return
    next_node = open_list.back();

    // Remove lowest cost node from open_list
    open_list.pop_back();

    return next_node;

}

// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found = {*current_node};
    RouteModel::Node * parent = nullptr;

    // TODO: Implement your solution here.
    while (true) {
        
        // Get parent
        parent = current_node->parent;

        // If parent doesn't exist, exit loop
        if (parent == nullptr)
            break;
        
        // Add parent to path
        path_found.insert(path_found.begin(), *parent);

        // Update cumulative distance    
        distance += current_node->distance(*parent);

        // Move to parent node for next iteration
        current_node = parent;
    }

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node;
    int i = 0;

    // TODO: Implement your solution here.
    
    // Add neighbors to open_list

    // Initialize starting node
    current_node = start_node;
    current_node->h_value = CalculateHValue(current_node);
    current_node->g_value = 0;
    current_node->visited = true;
    // open_list.push_back(current_node);
    // add node print function?

    // Sort open_list and return next node
    while (true) {

        AddNeighbors(current_node);
        current_node = NextNode();
        // std::cout << "distance to end = " << current_node->distance(*end_node) << std::endl;

        if (current_node == nullptr) {
            std::cout << "null pointer found: ";
            break;
        }
        if (current_node == end_node) { 
            // std::cout << "end node matched on iteration " << i << std::endl;
            // std::cout << "current_node-> x = " << current_node->x << " vs end_node->x = " << end_node->x << std::endl;
            // std::cout << "current_node-> y = " << current_node->y << " vs end_node->y = " << end_node->y << std::endl;
            break;
        }
        if (current_node->x == end_node->x && current_node->y == end_node->y) {
            std::cout << "end node x matches: ";
            break;
        }
        if (i == 500)
            break;
        i++;
    }

    // std::cout << "Constructing final path" << std::endl;

    // Construct finel path
    m_Model.path = ConstructFinalPath(current_node);

}