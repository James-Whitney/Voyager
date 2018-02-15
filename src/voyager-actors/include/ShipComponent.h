#pragma once
#ifndef _SHIPCOMPONENT_H
#define _SHIPCOMPONENT_H

#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>
#include <bullet/src/btBulletDynamicsCommon.h>

#include <voyager-physics/include/PhysicsComponent.h>

class ShipComponent : public Component {

public:

   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }
   void setPhysics(std::shared_ptr<PhysicsComponent> physicsComponent) { this->physicsComponent = physicsComponent; }

   void init();
   void update(double delta_time);


private:

   std::shared_ptr<PhysicsComponent> physicsComponent;
   void moveShip(float delta_time);

   std::shared_ptr<WindowManager> window;

};

#endif
