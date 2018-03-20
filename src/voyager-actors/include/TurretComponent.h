#pragma once
#ifndef _TURRETCOMPONENT_H
#define _TURRETCOMPONENT_H

#include <voyager-core/include/Application.h>

#include <voyager-render/include/Renderable.h>

#include <voyager-utils/include/BulletToGlm.h>

#include "ShipComponent.h"
#include "StationComponent.h"
#include "BulletComponent.h"

class TurretComponent : public StationComponent {

public:

   void init();
   void update(double delta_time);

   void setBulletType(int bullet_type) { this->bullet_type = bullet_type; }

   void setTurretID(int id) { this->id = id; }
   int getTurretID() { return id; }

   void fireBullet();

   void setApp(std::shared_ptr<Application> app) { this->app = app; }

private:

   int id;


   void logicUpdate(double delta_time);

   int bullet_type = 0;
   float fire_recoil = 0;

   std::shared_ptr<Application> app;

};

#endif
