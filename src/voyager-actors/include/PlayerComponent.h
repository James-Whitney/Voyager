#pragma once
#ifndef _PLAYERCOMPONENT_H
#define _PLAYERCOMPONENT_H

#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>

#include <voyager-utils/include/BulletToGlm.h>

#include <voyager-render/include/Camera.h>

#include <voyager-physics/include/PhysicsComponent.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "ShipComponent.h"

class PlayerComponent : public Component {

public:

   void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }
   void setShip(std::shared_ptr<Ship> ship) { this->ship = ship; }
   void setPhysics(std::shared_ptr<PhysicsComponent> physicsComponent) { this->physicsComponent = physicsComponent; }

   glm::vec3 getPosition();

   void init();
   void update(double delta_time);


private:

   std::shared_ptr<PhysicsComponent> physicsComponent;

   void cameraUpdate();
   void positionUpdate(float delta_time);

   std::shared_ptr<WindowManager> window;
   std::shared_ptr<Camera> camera;
   std::shared_ptr<Ship> ship;

   double prev_xPos = 0;
   double prev_yPos = 0;

};

#endif
