#pragma once
#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include <glm/glm/gtc/type_ptr.hpp>

#include <bullet/src/btBulletDynamicsCommon.h>
#include <bullet/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

#include <voyager-core/include/Component.h>

#include <voyager-core/include/Entity.h>

using namespace std;

class PhysicsComponent : public Component {

public:
   void init();
   void update(double delta_time);

   void initHeightMap(std::shared_ptr<Entity> entity, btVector3 position, btQuaternion rotation, int mapWidth, int mapLength, std::vector<unsigned char> heightfieldData, btScalar heightScale, btScalar minHeight, btScalar maxHeight, btScalar vertexSpace);

   void initRigidBody(std::shared_ptr<Entity> entity, 
                                       btCollisionShape *collisionShape, 
                                       btScalar mass, 
                                       btVector3 position, 
                                       btQuaternion rotation, 
                                       btVector3 velocity);


   btCollisionShape* get_collisionShape() {return collisionShape;}
   btRigidBody* getBody() { return body; }

private:

   //initialized variables, dont change.

   btCollisionShape        *collisionShape;
   btRigidBody             *body;
};

#endif
