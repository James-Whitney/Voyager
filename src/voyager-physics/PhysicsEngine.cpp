#include "include/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {

}

void PhysicsEngine::initConfigs() {
   //the broadphase, i dont know what tis thing does
   broadphase = new btDbvtBroadphase();
   //thing that decides how things collides
   collisionConfiguration = new btDefaultCollisionConfiguration();
   //collision dispatcher, there is a parallel version of this somewhere.
   dispatcher = new btCollisionDispatcher(collisionConfiguration);
   //this thing does the collison checking, there is aparallel version somewhere
   solver = new btSequentialImpulseConstraintSolver();
}
/*
void PhysicsEngine::initOverworld() {
   overworld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
   overworld->setGravity(btVector3(0, -9.8, 0));
}

void PhysicsEngine::initShipworld() {
   shipworld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
   shipworld->setGravity(btVector3(0, -9.8, 0));
}*/

void PhysicsEngine::initWorld() {
   world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
   world->setGravity(btVector3(0, -9.8, 0));
}

void PhysicsEngine::init() {

   initConfigs();
   initWorld();
   //initOverworld();
   //initShipworld();

   

}


void PhysicsEngine::execute(double delta_time) {
   ///-----stepsimulation_start-----
   world->stepSimulation(1, 10);

   for (int i = 0; i < components.size(); i++) {
      components[i]->update(delta_time);
   }

   //fprintf(stderr, "Num of Objects: %d\n", world->getNumCollisionObjects());
   
   //print positions of all objects
   
   for (int j = world->getNumCollisionObjects() - 1; j >= 0; j--)
   {
      btCollisionObject* obj = world->getCollisionObjectArray()[j];
      btRigidBody* body = btRigidBody::upcast(obj);
      btTransform trans;
      if (body && body->getMotionState())
      {
         body->getMotionState()->getWorldTransform(trans);
      }
      else
      {
         trans = obj->getWorldTransform();
      }
      printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
   }

}

void PhysicsEngine::addComponent(std::shared_ptr<PhysicsComponent> component){
   collisionShapes.push_back(component.get()->get_collisionShape());
   world->addRigidBody(component.get()->getBody());

}

void PhysicsEngine::removeComponent(PhysicsComponent* component) {
   
}