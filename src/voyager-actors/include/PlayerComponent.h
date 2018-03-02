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
#include "StationComponent.h"

class PlayerComponent : public ActorComponent {

public:

   void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
   void setShip(std::shared_ptr<ShipComponent> ship) { this->ship = ship; }
   void setPhysics(std::shared_ptr<PhysicsComponent> physicsComponent) { this->physicsComponent = physicsComponent; }

   void setHelm(std::shared_ptr<StationComponent> helm) { this->helm = helm; }
   void setTurret(std::shared_ptr<StationComponent> turret, int i);

   std::shared_ptr<PhysicsComponent> getPhysics() { return physicsComponent; }

   btScalar getRotation();

   glm::vec3 getPosition();

   void init();
   void update(double delta_time);


private:

   std::shared_ptr<PhysicsComponent> physicsComponent;
   std::shared_ptr<StationComponent> mounted;

   std::shared_ptr<StationComponent> helm;
   std::shared_ptr<StationComponent> turret0;
   std::shared_ptr<StationComponent> turret1;

   void stationSelectionCheck();
   void cameraUpdate();
   void positionUpdate(float delta_time);

   std::shared_ptr<Camera> camera;
   std::shared_ptr<ShipComponent> ship;

   bool isFalling = true;

   double prev_xPos = 0;
   double prev_yPos = 0;

   double prev_playerAngle = 0;
   double prev_shipAngle = 0;

   bool active = true;

};

#endif
