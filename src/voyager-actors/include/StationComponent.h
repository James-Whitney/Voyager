#pragma once
#ifndef _STATIONCOMPONENT_H
#define _STATIONCOMPONENT_H

#include <voyager-render/include/Camera.h>

#include <voyager-utils/include/BulletToGlm.h>

#include <bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include "ShipComponent.h"
//#include "ActorComponent.h"

class StationComponent : public ActorComponent {

public:

   int getHud() { return this->hud; }
   void setHud(int hud) { this->hud = hud; }

   void setShip(std::shared_ptr<ShipComponent> ship) { this->ship = ship; }   

   void activate() { this->active = true; }
   void deactivate() { this->active = false; }

   void setCameraHeight(float cameraHeight) { this->cameraHeight = cameraHeight; }

   void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }

   btScalar getRotation();

   void cameraUpdate(btScalar cameraHeight);
   

protected:

   bool active = false;

   int hud = 0;
   
   std::shared_ptr<ShipComponent> ship;

   std::shared_ptr<Camera> camera;

   float cameraHeight;
   float prev_StationAngle = 0.0;

};

#endif
