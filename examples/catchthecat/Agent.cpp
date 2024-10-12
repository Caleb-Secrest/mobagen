#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include "World.h"
using namespace std;

std::vector<Point2D> Agent::generatePath(World* w) {
    unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
    queue<Point2D> frontier;                   // to store next ones to visit
    unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
    unordered_map<Point2D, bool> visited;      // use .at() to get data; if the element doesn't exist, [] will give wrong results

    // bootstrap state
    auto catPos = w->getCat();
    frontier.push(catPos);
    frontierSet.insert(catPos);
    Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we don't find a border, we have to return random points

    while (!frontier.empty()) {
        // get the current from frontier
        Point2D current = frontier.front();
        frontier.pop();
        frontierSet.erase(current);  // remove the current from frontierSet
        visited[current] = true;      // mark current as visited

        // Get visitable neighbors
        vector<Point2D> neighbors = w->getVisitablePositions(current);

        for (const Point2D& neighbor : neighbors) {
            // Skip if the neighbor has already been visited or is in the frontier
            if (visited[neighbor] || frontierSet.count(neighbor)) {
                continue;
            }

            // Set the cameFrom for neighbor
            cameFrom[neighbor] = current;

            // If the neighbor is a border, update borderExit and break the loop
            if (w->isBorder(neighbor)) {
                borderExit = neighbor;
                break; // Break the loop since we found a border
            }

            // Enqueue the neighbor to frontier and frontierSet
            frontier.push(neighbor);
            frontierSet.insert(neighbor);
        }

        if (borderExit != Point2D::INFINITE) {
            break; // Exit while loop if a border is found
        }
    }

    // If the border is not infinity, build the path from border to the cat using the cameFrom map
    if (borderExit != Point2D::INFINITE) {
        vector<Point2D> path;
        for (Point2D at = borderExit; at != catPos; at = cameFrom[at]) {
            path.push_back(at);
        }
        path.push_back(catPos); // add the cat's position
        reverse(path.begin(), path.end()); // Reverse the path to get from cat to border
        return path;
    }

    // If there isn't a reachable border, just return an empty vector
    return vector<Point2D>();
}
