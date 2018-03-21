#pragma once
#ifndef _SPAWNER_H
#define _SPAWNER_H

#include "Drone.h"

class Spawner : public Component, public std::vector<btVector3> {
public:
   virtual void init() override;
   virtual void update(double delta_time) override;

   void setScene(std::shared_ptr<Scene> s) { this->scene = s; }
   void setNavMap(std::shared_ptr<NavMap> nm) { this->nav_map = nm; }
   void setApplication(std::shared_ptr<Application> app) { this->app = app; }

   float getFrequency() { return this->freq; }
   void setFrequency(float f) { this->freq= f; }

protected:
   std::shared_ptr<Scene> scene;
   std::shared_ptr<NavMap> nav_map;
   std::shared_ptr<Application> app;

   float timer = 0;
   float freq;

   void spawnDrone();
};

#endif
