#pragma once
#ifndef _BULLETCOMPONENT_H
#define _BULLETCOMPONENT_H

#include <voyager-core/include/Application.h>

#include "ActorComponent.h"

class BulletComponent : public ActorComponent {

public:

   void init();
   void update(double delta_time);

   void setBulletType(int bullet_type) { this->bullet_type = bullet_type; }
   int getBulletType() { return this->bullet_type; }

   void setApp(std::shared_ptr<Application> app) { this->app = app; }

private:

   int id;

   void logicUpdate(double delta_time);
   void removeBullet();

   int bullet_type = 0;

   float life_time = 0.0;

   std::shared_ptr<Application> app;   

};

#endif
