#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

// #include "utils/Ray.h"
// #include "utils/Shape.h"

class Entity {

public:
   Entity();

   void update();
   void render();

private:

   // Ray orientation;
   // std::shared_ptr<Shape> shape;

};

#endif
