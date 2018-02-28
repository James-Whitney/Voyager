#pragma once
#ifndef _SHIPCOMPONENT_H
#define _SHIPCOMPONENT_H

#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>
#include <bullet/src/btBulletDynamicsCommon.h>

#include "ActorComponent.h"

#include <voyager-physics/include/PhysicsComponent.h>

class PhysicsComponent;

class ShipComponent : public ActorComponent {

public:

   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }
   void setPhysics(std::shared_ptr<PhysicsComponent> physicsComponent) { this->physicsComponent = physicsComponent; }
   std::shared_ptr<PhysicsComponent> getPhysics() { return physicsComponent; }

   btScalar getRotation();
   btVector3 getRotationVector();

   void init();
   void update(double delta_time);

   btScalar getMaxVertThrottle() { return this->max_vertThrottle; }
   btScalar getMinVertThrottle() { return this->min_vertThrottle; }
   btScalar getMaxForwardThrottle() { return this->max_forwardThrottle; }
   btScalar getMinForwardThrottle() { return this->min_forwardThrottle; }
   btScalar* getVertThrottle() { return &(this->vertThrottle); }
   btScalar* getForwardThrottle() { return &(this->forwardThrottle); }

private:

   const btScalar max_vertThrottle = 100;
   const btScalar min_vertThrottle = -100;

   const btScalar max_forwardThrottle = 100;
   const btScalar min_forwardThrottle = -100;

   btScalar vertThrottle = 0;
   btScalar forwardThrottle = 0;

   std::shared_ptr<PhysicsComponent> physicsComponent;
   void moveShip(float delta_time);

   std::shared_ptr<WindowManager> window;

};

#endif
