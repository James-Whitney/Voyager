#pragma once
#ifndef _DRONE_H
#define _DRONE_H

#include <voyager-core/include/Entity.h>
#include <voyager-loader/include/Scene.h>

#include "../ai/NavMap.h"
#include "../ai/BrainComponent.h"

struct DronePhysicsProperties {

   // Physical Properties of Drones
   btScalar lin_damp = 0.1f;
   btScalar ang_damp = 0.1f;
   btScalar friction = 1.0f;
   btScalar mass = 800.0f;
   // btBoxShape *shape = new btBoxShape(btVector3(4, 4, 4));

   btScalar box_size = 4.0f;

   static std::shared_ptr<PhysicsComponent> toPhysicsComponent(std::shared_ptr<Entity> entity);
};

class Drone : public Entity {
public:
   Drone(std::shared_ptr<Scene> scene, std::shared_ptr<NavMap> nav_map,
      std::shared_ptr<btTransform> trans);

   void initPhysics();

   void linkComponents();

private:

   std::shared_ptr<Renderable> renderable;
   std::shared_ptr<BrainComponent> brain;
   std::shared_ptr<PhysicsComponent> physics;

   std::shared_ptr<NavMap> nav_map;
};

#endif
