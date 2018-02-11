#include "include/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {

}

void PhysicsEngine::init() {
   //1
   _broadphase = new btDbvtBroadphase();
   //2
   _collisionConfiguration = new btDefaultCollisionConfiguration();
   _dispatcher = new btCollisionDispatcher(_collisionConfiguration);
   //3
   _solver = new btSequentialImpulseConstraintSolver();
   //4
   _world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
   //5
   _world->setGravity(btVector3(0, -9.8, 0));

   _world->add
}

void PhysicsEngine::execute(double delta_time) {

}