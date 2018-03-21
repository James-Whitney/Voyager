#include "include/PhysicsComponent.h"
#include <iostream>

void PhysicsComponent::init() {

}

void PhysicsComponent::initHeightMap(std::shared_ptr<Entity> entity, btVector3 position, btQuaternion rotation, btHeightfieldTerrainShape *collisionShape) {
   this->entity = entity;
   this->world = 1;
   this->terrain = true;

   std::shared_ptr<btTransform> transform = entity->getTransform();
   transform->setIdentity();
   transform->setOrigin(position); // Set Position
   btDefaultMotionState * myMotionState = new btDefaultMotionState(*(transform.get())); // Create Motion State

   // btDefaultMotionState* myMotionState = new btDefaultMotionState(*(transform.get()));
   //btHeightfieldTerrainShape *heightFieldShape = new btHeightfieldTerrainShape(mapWidth, mapLength, (void *)(&heightfieldData[0]), 1, -1024, 1016, 2, PHY_UCHAR, true);
   //btVector3 localScale = btVector3(mapWidth / 127.0, mapLength / 127.0, 8);
   // TODO: bring this back
   // btVector3 localScale = btVector3(0.25, 0.25, 0.25);
   //collisionShape->setUseDiamondSubdivision(true);
   collisionShape->setLocalScaling(btVector3(1.0, 1.0, 1.0));
   // collisionShape->processAllTriangles();
   //collisionShape->setLocalScaling(btVector3());

   btRigidBody::btRigidBodyConstructionInfo rbInfo(btScalar(0.0), myMotionState, collisionShape, btVector3(0.0, 0.0, 0.0));

   this->body = new btRigidBody(rbInfo);
   // this->body->setActivationState(DISABLE_DEACTIVATION);
}


void PhysicsComponent::initRigidBody(int world, std::shared_ptr<Entity> entity, btCollisionShape *collisionShape, btScalar mass, btVector3 position, btQuaternion rotation, btVector3 velocity, btScalar friction) {

   this->world = world;
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
   rbInfo.m_friction = friction;
   this->body = new btRigidBody(rbInfo);
   
   entity->setTransform(transform);
   
   //this->body->applyGravity();
   this->body->setSleepingThresholds(btScalar(0.001), btScalar(0.001));
   
}

void PhysicsComponent::update(double delta_time) {

}

void PhysicsComponent::updatePosition(std::shared_ptr<ShipComponent> ship) {
   btTransform objectTrans;
   btTransform shipTrans;
   
   body->getMotionState()->getWorldTransform(objectTrans);
   if (world == 0) {
      ship->getPhysics()->getBody()->getMotionState()->getWorldTransform(shipTrans);
      btVector3 position = objectTrans.getOrigin().rotate(btVector3(0, 1, 0), ship->getRotation());
      position += shipTrans.getOrigin();
      objectTrans.setOrigin(position);
      btQuaternion rotation = objectTrans.getRotation() * shipTrans.getRotation();
      objectTrans.setRotation(rotation);
   }
   if (terrain) {
      // btVector3 temp = objectTrans->getOrigin();
      // objectTrans->setOrigin(temp.getX() / 2.0f, temp.getY() / 2.0f, temp.getZ() / 2.0f);
      objectTrans.setOrigin(btVector3(0.0, 0.0, 0.0));
   }
   entity->setTransform(std::make_shared<btTransform>(objectTrans));
}