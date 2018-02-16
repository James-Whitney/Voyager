#include "include/PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::init() {

}

void PhysicsComponent::initHeightMap(std::shared_ptr<Entity> entity, btVector3 position, btQuaternion rotation, int mapWidth, int mapLength, std::vector<unsigned char> heightfieldData, btScalar heightScale, btScalar minHeight, btScalar maxHeight, btScalar vertexSpace) {
   this->entity = entity;

   std::shared_ptr<btTransform> transform = entity->getTransform();

   btDefaultMotionState* myMotionState = new btDefaultMotionState(*(transform.get()));

   this->collisionShape = new btHeightfieldTerrainShape(mapWidth,
                                                         mapLength,
                                                         (void *)(&heightfieldData[0]),
                                                         heightScale,
                                                         minHeight,
                                                         maxHeight,
                                                         1, PHY_UCHAR, false);
   
   collisionShape->setLocalScaling(btVector3(1.0, vertexSpace, 10.0));

   btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(0.0), myMotionState, collisionShape, btVector3(0.0, 0.0, 0.0));

   this->body = new btRigidBody(rbInfo);   
                                          
}


void PhysicsComponent::initRigidBody(std::shared_ptr<Entity> entity, btCollisionShape *collisionShape, btScalar mass, btVector3 position, btQuaternion rotation, btVector3 velocity) {

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
   transform->setRotation(rotation);
   
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