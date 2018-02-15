#pragma once
#ifndef _PHYSICSENGINE_H
#define _PHYSICSENGINE_H

#include <voyager-core/include/Engine.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include "PhysicsComponent.h"

class PhysicsEngine : public Engine {

public:

   PhysicsEngine();

   void init();
   void initConfigs();
   //void initOverworld();
   //void initShipworld();
   void initWorld();

   btDiscreteDynamicsWorld* get_world() { return world; }

   //btDiscreteDynamicsWorld* get_shipworld() {return shipworld;}

   void execute(double delta_time = 0);

   void removeComponent(PhysicsComponent* component);

   void addEntity();

   void updateEntityTransforms();


protected:

   //Overworld Objects
   btBroadphaseInterface                  *broadphase;
   btDefaultCollisionConfiguration        *collisionConfiguration;
   btCollisionDispatcher                  *dispatcher;
   btSequentialImpulseConstraintSolver    *solver;

   btDiscreteDynamicsWorld                *world;
   btAlignedObjectArray<btCollisionShape*> collisionShapes;

};

#endif
