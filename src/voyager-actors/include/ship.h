#pragma once
#ifndef _SHIP_H
#define _SHIP_H

#include <voyager-core/include/Entity.h>
#include <voyager-core/include/WindowManager.h>

class Ship : public Entity {

public:

   void setWindow(std::shared_ptr<WindowManager> window) { this->window = window; }

   float getShipAngle() { return angle; }

   void init();
   void update(double delta_time);

private:

   void moveShip(float delta_time);
   double angle = 0;

   std::shared_ptr<WindowManager> window;

   
};

#endif
