#pragma once
#ifndef _ACTORENGINE_H
#define _ACTORENGINE_H

#include <iostream>
#include <memory>

#include <voyager-core/include/Engine.h>

#include "ActorComponent.h"

#include <bullet/src/btBulletDynamicsCommon.h>

#include "ai/NavMap.h"

class ActorEngine : public Engine {

public:

   void init();
   void execute(double delta_time = 0);
   virtual void removeFlagged();

protected:

};

#endif
