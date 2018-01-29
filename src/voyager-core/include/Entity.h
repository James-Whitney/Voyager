#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <voyager-utils/include/Ray.h>
#include <voyager-utils/include/Shape.h>

class Entity {

public:
   Entity();

   void update();
   void render();

private:

   Ray orientation;
   std::shared_ptr<Shape> shape;

};

#endif
