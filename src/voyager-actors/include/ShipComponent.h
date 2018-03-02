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

   void turnShip(btScalar angle);

   void raiseShip(btScalar rise);

   void moveShip(float delta_time); 

   void setForwardThrottle(btScalar forwardThrottle) { this->forwardThrottle = forwardThrottle; }

   void setSpeed(btScalar speed) { this->speed = speed; }

private:

   btScalar speed = 0.0;

   btScalar       forwardThrottle;
   std::shared_ptr<PhysicsComponent> physicsComponent;

   std::shared_ptr<WindowManager> window;

};

#endif
