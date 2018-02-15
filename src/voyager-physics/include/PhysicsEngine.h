#pragma once
#ifndef _PHYSICSENGINE_H
#define _PHYSICSENGINE_H

#include <voyager-core/include/Engine.h>

#include "PhysicsComponent.h"

class PhysicsEngine : public Engine {

public:

   PhysicsEngine();

   void init();
   void execute(double delta_time = 0);


protected:



};

#endif
