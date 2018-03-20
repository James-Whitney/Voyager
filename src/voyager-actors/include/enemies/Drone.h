#pragma once
#ifndef _DRONE_H
#define _DRONE_H

#include <voyager-core/include/Entity.h>
#include <voyager-loader/include/Scene.h>

#include "../ai/NavMap.h"
#include "../ai/BrainComponent.h"

class Drone : public Entity {
public:
   Drone(std::shared_ptr<Scene> scene, std::shared_ptr<NavMap> nav_map,
      std::shared_ptr<btTransform> trans);

   void linkComponents();

private:

   std::shared_ptr<Renderable> renderable;
   std::shared_ptr<BrainComponent> brain;

   std::shared_ptr<NavMap> nav_map;
};

#endif
