#pragma once
#ifndef _STATIONCOMPONENT_H
#define _STATIONCOMPONENT_H

#include <voyager-render/include/Camera.h>

class StationComponent : public ActorComponent {

public:

   int getHud() { return this->hud; }
   void setHud(int hud) { this->hud = hud; }

private:

   int hud = 0;
   bool active = false;
   std::shared_ptr<Camera> camera;


};

#endif
