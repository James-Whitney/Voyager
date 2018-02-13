#pragma once
#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include <glm/glm/gtc/type_ptr.hpp>

#include <bullet/src/btBulletDynamicsCommon.h>

#include <voyager-core/include/Component.h>

#include <voyager-core/include/Entity.h>

class PhysicsComponent : public Component {

public:
   void init();
   void update(double delta_time);

   void initRigidBody(std::shared_ptr<Entity> entity,
                      btCollisionShape *collisionShape, 
                      btScalar mass, 
                      btVector3 position, 
                      btVector3 velocity);

   btCollisionShape* get_collisionShape() {return collisionShape;}
   btRigidBody* getBody() { return body; }

private:

   btCollisionShape        *collisionShape;
   btDefaultMotionState    *myMotionState;
   btRigidBody             *body;
};

#endif
