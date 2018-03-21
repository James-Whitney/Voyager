#pragma once
#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include <glm/glm/gtc/type_ptr.hpp>

#include <bullet/src/btBulletDynamicsCommon.h>
#include <bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <voyager-core/include/Component.h>
#include <voyager-core/include/Entity.h>

#include <voyager-actors/include/ShipComponent.h>

class ShipComponent;



using namespace std;

class PhysicsComponent : public Component {

public:
   void init();
   void update(double delta_time);

   void initHeightMap(std::shared_ptr<Entity> entity, btVector3 position, btQuaternion rotation, btHeightfieldTerrainShape *collisionShape) ;

   void initRigidBody(  int world, 
                        std::shared_ptr<Entity> entity, 
                        btCollisionShape *collisionShape, 
                        btScalar mass, 
                        btVector3 position, 
                        btQuaternion rotation, 
                        btVector3 velocity,
                        btScalar friction);                

   btCollisionShape* get_collisionShape() {return collisionShape;}
   btRigidBody*      getBody() { return body; }

   int getWorldIndex() { return world; }

   void updatePosition(std::shared_ptr<ShipComponent> ship);

private:

   //initialized variables, dont change.
   int                     world;

   btCollisionShape        *collisionShape;
   btRigidBody             *body;
   bool terrain = false;
};

#endif
