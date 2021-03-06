#pragma once
#ifndef _HELMCOMPONENT_H
#define _HELMCOMPONENT_H

#include "StationComponent.h"


class HelmComponent : public StationComponent {

public:

   void init();
   void update(double delta_time);

   btScalar getMaxForwardThrottle() { return this->max_forwardThrottle; }
   btScalar getMinForwardThrottle() { return this->min_forwardThrottle; }
   btScalar* getForwardThrottle() { return &(this->forwardThrottle); }

   void setTurnSpeed(btScalar turnSpeed) { this->turnSpeed = turnSpeed; }
   void setRiseSpeed(btScalar riseSpeed) { this->riseSpeed = riseSpeed; }
private:

   

   void logicUpdate(double delta_time);

   btScalar forwardThrottle = 0.0;

   btScalar turnSpeed = 0.0;
   btScalar riseSpeed = 0.0;

   const btScalar max_forwardThrottle = 100;
   const btScalar min_forwardThrottle = -100;

};

#endif
