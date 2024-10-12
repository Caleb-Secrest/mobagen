#ifndef WORLD_H
#define WORLD_H

#include "Cat.h"
#include "Catcher.h"
#include "scene/GameObject.h"
#include "math/Point2D.h"
#include "Random.h"
#include <bitset>
#include <iostream>
#include <vector>

class World : GameObject {
private:
  float timeBetweenAITicks = 1;
  float timeForNextTick = 1;
  bool catTurn = true;
  bool isSimulating = false;
  Point2D catPosition = Point2D(0, 0);
  int64_t moveDuration = 0;
  bool catWon = false;
  bool catcherWon = false;

  Cat* cat;
  Catcher* catcher;

  // false means empty
  // true means blocked
  std::vector<bool> worldState;

  // size of the side of the map
  int sideSize = 0;

  // clears the world
  void clearWorld();

  // check if cat won
  bool catWinVerification();

  // check if catcher won
  bool catcherWinVerification();

public:
  explicit World(Engine* pEngine, int size = 11);
  explicit World(Engine* pEngine, int mapSideSize, bool isCatTurn, Point2D catPos, std::vector<bool> map);

  ~World();

  // directions
  static Point2D NE(const Point2D& p);
  static Point2D NW(const Point2D& p);
  static Point2D E(const Point2D& p);
  static Point2D W(const Point2D& p);
  static Point2D SE(const Point2D& p);
  static Point2D SW(const Point2D& p);

  // returns the cat position
  Point2D getCat();

  // returns the side of the map
  int getWorldSideSize();

  // the top left (x,y) is (-side/2,-side/2) the center is on (0,0);
  // get the content of a given point
  bool getContent(const Point2D& p) const { return worldState[(p.y + sideSize / 2) * (sideSize) + p.x + sideSize / 2]; }

  // the top left (x,y) is (-5,-5) the center is on (0,0);
  // get the content of a given
  bool getContent(const int& x, const int& y) const { return worldState[(y + sideSize / 2) * (sideSize) + x + sideSize / 2]; }

  // print the world state
  void print();

  // check if point is inside the world
  bool isValidPosition(const Point2D& p);

  static bool isNeighbor(const Point2D& p1, const Point2D& p2);

  void OnDraw(SDL_Renderer* renderer) override;
  void OnGui(ImGuiContext* context) override;
  void Update(float deltaTime) override;

  void step();

  // check if cat can move to the position required
  bool catCanMoveToPosition(Point2D pos) const;

  // check if catcher can move to the position required
  bool catcherCanMoveToPosition(Point2D pos) const;

  // returns true if cat wins on the given space
  bool catWinsOnSpace(Point2D point);

  static std::vector<Point2D> neighbors(Point2D point) {
    std::vector<Point2D> n;
    n.push_back(NE(point));
    n.push_back(NW(point));
    n.push_back(E(point));
    n.push_back(W(point));
    n.push_back(SW(point));
    n.push_back(SE(point));
    return n;
  }

  std::vector<Point2D> getVisitablePositions(const Point2D& current)
  {
    std::vector<Point2D> visitable;

    std::vector<Point2D> directions = {
    {0, 1},   // North
    {1, 1},   // Northeast
    {1, 0},   // East
    {1, -1},  // Southeast
    {0, -1},  // South
    {-1, -1}, // Southwest
    {-1, 0},  // West
    {-1, 1}   // Northwest
    };

    for (const auto& dir :directions) {
      Point2D neighbor = current + dir;
      if (isWithinBounds(neighbor) && !getContent(neighbor)) {
      visitable.push_back(neighbor);
      }
    }

    return visitable;
  }

  bool isWithinBounds(const Point2D& p) const{
    int halfSize = sideSize / 2;
    return p.x >= -halfSize && p.x < halfSize && p.y >= -halfSize && p.y < halfSize;
  }

  bool isBorder(const Point2D& p) const;

  Point2D reverse(const Point2D& dir) const;

  bool isBlocked(const Point2D& p) const;

};

#endif  // WORLD_H
