#pragma once
#ifndef _DRONE_H
#define _DRONE_H

#include <voyager-core/include/Entity.h>
#include <voyager-loader/include/Scene.h>

#include "../ai/NavMap.h"
#include "../ai/BrainComponent.h"
#include "Enemy.h"

struct DronePhysicsProperties {

   // Physical Properties of Drones
   btScalar lin_damp = 0.1f;
   btScalar ang_damp = 0.1f;
   btScalar friction = 1.0f;
   btScalar mass = 200.0f;
   btScalar box_size = 4.0f;

   static std::shared_ptr<PhysicsComponent> toPhysicsComponent(std::shared_ptr<Entity> entity);
};

class Drone : public Entity, public Enemy {
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

class DroneBrain : public Brain {
public:
   DroneBrain(std::shared_ptr<Entity> player);
};

class DroneDoNothing : public DoNothing {
public:
   DroneDoNothing(std::shared_ptr<Entity> player) : DoNothing(player) {}
protected:
   virtual void doNothing(double delta_time) override;
};

class DroneChase : public Chase {
public:
   DroneChase(std::shared_ptr<Entity> player) : Chase(player) {}
protected:
   virtual void chase(double delta_time) override;
};

#endif
