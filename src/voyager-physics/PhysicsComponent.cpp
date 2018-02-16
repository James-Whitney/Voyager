#include "include/PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::init() {

}

void PhysicsComponent::initRigidBody(std::shared_ptr<Entity> entity, btCollisionShape *collisionShape, btScalar mass, btVector3 position, btVector3 velocity) {

   this->entity = entity;
   //create a dynamic rigidbody
   //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
   
   this->collisionShape = collisionShape;
   
   /// Create Dynamic Objects
   std::shared_ptr<btTransform> transform = std::make_shared<btTransform>();
   transform->setIdentity();
   
   //rigidbody is dynamic if and only if mass is non zero, otherwise static
   bool isDynamic = (mass != 0.f);
   
   if (isDynamic)
      collisionShape->calculateLocalInertia(mass, velocity);
   
   transform->setOrigin(position);
   
   //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
   btDefaultMotionState* myMotionState = new btDefaultMotionState(*(transform.get()));
   
   btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, velocity);
   
   this->body = new btRigidBody(rbInfo);
   
   entity->setTransform(transform);
   
   //this->body->applyGravity();
   this->body->setSleepingThresholds(btScalar(0.001), btScalar(0.001));
   
}

void PhysicsComponent::update(double delta_time) {
   btTransform trans;
   body->getMotionState()->getWorldTransform(trans);
   entity->setTransform(std::make_shared<btTransform>(trans));
}