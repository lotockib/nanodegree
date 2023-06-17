// Copyright 2023

#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(
    RouteModel &model,
    float start_x,
    float start_y,
    float end_x,
    float end_y) : m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Find closest existing nodes to user-provided start and end coordinates
    start_node = &model.FindClosestNode(start_x, start_y);
    end_node = &model.FindClosestNode(end_x, end_y);
}


float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    // Find distance from current node to end node
    return node->distance(*end_node);
}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    // Find neighbors of current node and populate vector
    current_node->FindNeighbors();

    // For each neightbor node
    for (auto& neighbor : current_node->neighbors) {
        // Don't process if already visited
        if (neighbor->visited != true) {
            // Set parent to current
            neighbor->parent = current_node;

            // Find distance between current node and this neighbor
            neighbor->g_value =
                current_node->g_value + current_node->distance(*neighbor);

            // Find h value
            neighbor->h_value = RoutePlanner::CalculateHValue(neighbor);

            // Mark visited`
            neighbor->visited = true;

            // Add to the open list
            open_list.push_back(neighbor);
        }
    }
}


bool Compare(RouteModel::Node * node0, RouteModel::Node * node1) {
    // Return true if node0 is closer, false otherwise
    return node0->g_value + node0->h_value > node1->g_value + node1->h_value;
}


RouteModel::Node *RoutePlanner::NextNode() {
    // Initialize variables
    RouteModel::Node * next_node = nullptr;

    // Sort open_list Option A: use dedicated function
    // sort(open_list.begin(), open_list.end(), Compare);

    // Sort open_list option B: use lambda (better)
    sort(
        open_list.begin(),
        open_list.end(),
        []
        (const RouteModel::Node* a, const RouteModel::Node* b)
        {return a->g_value + a->h_value > b->g_value + b->h_value;
        });

    // Get lowest cost node for return
    next_node = open_list.back();
    open_list.pop_back();

    return next_node;
}


std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(
    RouteModel::Node *current_node) {

    // Initialize variables
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found = {*current_node};
    RouteModel::Node * parent = nullptr;

    // Step through each parent to construct start-to-end path vector
    while (true) {
        // Get parent
        parent = current_node->parent;

        // If parent doesn't exist, we're at the starting node, exit loop
        if (parent == nullptr)
            break;

        // Add parent to path option A
        // path_found.insert(path_found.begin(), *parent);

        // Add parent to path option B, more memory efficient per big O notation
        path_found.push_back(*parent);

        // Update cumulative distance
        distance += current_node->distance(*parent);

        // Move to parent node for next iteration
        current_node = parent;
    }

    // Convert to meters, and return path in descending order
    distance *= m_Model.MetricScale();
    std::reverse(path_found.begin(), path_found.end());
    return path_found;
}


void RoutePlanner::AStarSearch() {
    // Initialize starting node
    RouteModel::Node *current_node;
    current_node = start_node;
    current_node->h_value = CalculateHValue(current_node);
    current_node->g_value = 0;
    current_node->visited = true;

    // Sort open_list and return next node
    while (true) {
        // Add neighbors of current_node to open_list
        AddNeighbors(current_node);

        // Sort open list and find next node
        current_node = NextNode();

        // Check if at end_node
        if (current_node == end_node)
            break;
    }

    // Construct finel path
    m_Model.path = ConstructFinalPath(current_node);
}
