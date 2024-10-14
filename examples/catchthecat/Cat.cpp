#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <iostream>

Point2D Cat::Move(World* world) {
  auto catPos = world->getCat();

  if (!generatePath(world).empty())
    return generatePath(world).back();
  else
    return catPos;
}
