#pragma once
#ifndef _PHYSICSENGINE_H
#define _PHYSICSENGINE_H

#include <voyager-core/include/Engine.h>

#include <bullet/btBulletDynamicsCommon.h>

#include "PhysicsComponent.h"

class PhysicsEngine : public Engine {

public:

   PhysicsEngine();

   void init();
   void execute(double delta_time = 0);


protected:

   btBroadphaseInterface                  *_broadphase;
   btDefaultCollisionConfiguration        *_collisionConfiguration;
   btCollisionDispatcher                  *_dispatcher;
   btSequentialImpulseConstraintSolver    *_solver;
   btDiscreteDynamicsWorld                *_world;


};

#endif
