#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm> // Include this for std::reverse
#include "World.h"
using namespace std;

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;
  queue<Point2D> frontier;
  unordered_set<Point2D> frontierSet;
  unordered_map<Point2D, bool> visited;

  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;

  while (!frontier.empty()) {
    Point2D current = frontier.front();
    frontier.pop();
    frontierSet.erase(current);
    visited[current] = true;

    // Retrieve neighbors and check if they are valid
    auto neighbors = w->getVisitablePositions(current);
    for (const auto& neighbor : neighbors) {
      if (visited.find(neighbor) == visited.end() && !w->getContent(neighbor)) {
        cameFrom[neighbor] = current;
        frontier.push(neighbor);
        frontierSet.insert(neighbor);

        if (w->isBorder(neighbor)) {
          borderExit = neighbor;
          break; // Found a border, exit loop
        }
      }
    }

    if (borderExit != Point2D::INFINITE) break; // Exit outer loop if border found
  }

  // Rebuild the path from border to cat
  vector<Point2D> path;
  if (borderExit != Point2D::INFINITE) {
    for (Point2D at = borderExit; at != catPos; at = cameFrom.at(at)) {
      path.push_back(at);
    }
    path.push_back(catPos);
    std::reverse(path.begin(), path.end()); // Fully qualify the reverse function
  }

  return path; // Return path or empty if no path found
}
