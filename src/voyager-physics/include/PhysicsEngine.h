#pragma once
#ifndef _PHYSICSENGINE_H
#define _PHYSICSENGINE_H

#include <voyager-core/include/Engine.h>

#include <bullet/src/btBulletDynamicsCommon.h>

#include <voyager-actors/include/PlayerComponent.h>
#include <voyager-actors/include/ShipComponent.h>

#include "PhysicsComponent.h"

#include <iostream>

class PhysicsEngine : public Engine {

public:

   void init();
   void initConfigs();
   //void initOverworld();
   //void initShipworld();
   void initWorld();
   void initShipWorld();

   btDiscreteDynamicsWorld* get_world() { return this->world; }
   btDiscreteDynamicsWorld* get_ship_world() { return this->ship_world; }

   void setShip(std::shared_ptr<ShipComponent> ship) { this->ship = ship; }

   std::shared_ptr<Entity> findEntityFromBody(const btCollisionObject* ob);

   void checkCollision(btDiscreteDynamicsWorld *this_world);

   void clearEntityCollides();
   
   //btDiscreteDynamicsWorld* get_shipworld() {return shipworld;}

   void execute(double delta_time = 0);

   void removeComponent(PhysicsComponent* component);

   void addEntity();

   void updateEntityTransforms();

   btDiscreteDynamicsWorld* getWorld() { return world; }


   void registerComponent(std::shared_ptr<Component> component);
   

protected:

   std::shared_ptr<ShipComponent>         ship;
   // ***** Ship World ***** //
   btBroadphaseInterface                  *ship_broadphase;
   btDefaultCollisionConfiguration        *ship_collisionConfiguration;
   btCollisionDispatcher                  *ship_dispatcher;
   btSequentialImpulseConstraintSolver    *ship_solver;
   btAxisSweep3                           *ship_overlappingPairCache;
   btDiscreteDynamicsWorld                *ship_world;

   // ***** World ***** //
   btBroadphaseInterface                  *broadphase;
   btDefaultCollisionConfiguration        *collisionConfiguration;
   btCollisionDispatcher                  *dispatcher;
   btSequentialImpulseConstraintSolver    *solver;
   btAxisSweep3                           *overlappingPairCache;
   btDiscreteDynamicsWorld                *world;

   btAlignedObjectArray<btCollisionShape*> collisionShapes;
   
};

#endif
