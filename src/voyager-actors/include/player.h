#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>

#include <voyager-render/include/Camera.h>

#include "Ship.h"

class Player : public Entity {

public:

   Player();

   void setCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }
   void setShip(std::shared_ptr<Ship> ship) { this->ship = ship; }

   glm::vec3 getPosition();

   void init();
   void update(double delta_time);

private:

   void cameraUpdate();
   void positionUpdate(float delta_time);

   std::shared_ptr<WindowManager> window;
   std::shared_ptr<Camera> camera;
   std::shared_ptr<Ship> ship;

   double prev_xPos = 0;
   double prev_yPos = 0;
   float prev_shipAngle = 0;

};

#endif
