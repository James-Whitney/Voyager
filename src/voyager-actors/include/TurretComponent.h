#pragma once
#ifndef _TURRETCOMPONENT_H
#define _TURRETCOMPONENT_H

#include "ShipComponent.h"
#include "StationComponent.h"

class TurretComponent : public StationComponent {

public:

   void init();
   void update(double delta_time);

   void setShip(std::shared_ptr<ShipComponent> ship) { this->ship = ship; }

   void setBulletType(int bullet_type) { this->bullet_type = bullet_type; }

   void setTurretID(int id) { this->id = id; }
   int getTurretID() { return id; }

private:

   int id;

   std::shared_ptr<ShipComponent> ship;

   void logicUpdate(double delta_time);

   int bullet_type = 0;

};

#endif
