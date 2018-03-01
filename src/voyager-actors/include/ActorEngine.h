#pragma once
#ifndef _ACTORENGINE_H
#define _ACTORENGINE_H

#include <iostream>
#include <memory>

#include <voyager-core/include/Engine.h>
#include <voyager-core/include/Entity.h>

#include <bullet/src/btBulletDynamicsCommon.h>

class ActorEngine : public Engine {

public:

   void init();
   void execute(double delta_time = 0);

protected:

};

#endif
