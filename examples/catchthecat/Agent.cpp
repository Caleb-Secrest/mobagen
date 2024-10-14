#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
#include <vector>

using namespace std;

struct a_star_node {
    Point2D p;
    int h, accumDist;

    bool operator<(const a_star_node& other) const {
        return (h + accumDist) > (other.h + other.accumDist);
    }
};

int calculateHeuristics(const Point2D& p, int gridSize) {
    return min(gridSize - abs(p.x), gridSize - abs(p.y));
}

vector<Point2D> Agent::generatePath(World* w) {
    unordered_map<Point2D, Point2D> cameFrom;
    priority_queue<a_star_node> frontier;
    unordered_set<Point2D> frontierSet;
    unordered_set<Point2D> visited;

    auto catPos = w->getCat();
    frontier.push({catPos, 0, 0});
    frontierSet.insert(catPos);
    Point2D exit = Point2D::INFINITE;

    int gridSize = w->getWorldSideSize() / 2;

    while (!frontier.empty()) {
        a_star_node currPos = frontier.top();
        frontier.pop();
        frontierSet.erase(currPos.p);
        visited.insert(currPos.p);

        if (currPos.p.x == -(gridSize) || currPos.p.x == gridSize || currPos.p.y == -(gridSize) || currPos.p.y == gridSize) {
            exit = currPos.p;
            break;
        }

        vector<Point2D> neighbors = w->neighbors(currPos.p);

        for (const Point2D& neighbor : neighbors) {
            if (!w->getContent(neighbor) && visited.find(neighbor) == visited.end() && w->isValidPosition(neighbor)) {
                cameFrom[neighbor] = currPos.p;
                a_star_node neighborNode = {neighbor, calculateHeuristics(neighbor, gridSize), currPos.accumDist + 1};
                frontier.push(neighborNode);
                frontierSet.insert(neighbor);
            }
        }
    }

    vector<Point2D> path;
    while (exit != catPos && exit != Point2D::INFINITE) {
        path.push_back(exit);
        exit = cameFrom[exit];
    }

    return path;
}
