#include "include/PhysicsEngine.h"

void PhysicsEngine::initWorld() {
   //the broadphase, i dont know what tis thing does
   broadphase = new btDbvtBroadphase();
   //thing that decides how things collides
   collisionConfiguration = new btDefaultCollisionConfiguration();
   //collision dispatcher, there is a parallel version of this somewhere.
   dispatcher = new btCollisionDispatcher(collisionConfiguration);
   //this thing does the collison checking, there is aparallel version somewhere
   solver = new btSequentialImpulseConstraintSolver();

   btVector3 worldMin(-100,-100,-100);
	btVector3 worldMax(100,100,100);
	overlappingPairCache = new btAxisSweep3(worldMin,worldMax);

   //World setup
   world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
   world->setGravity(btVector3(0, 0, 0));
}

void PhysicsEngine::initShipWorld() {
   //the broadphase, i dont know what tis thing does
   ship_broadphase = new btDbvtBroadphase();
   //thing that decides how things collides
   ship_collisionConfiguration = new btDefaultCollisionConfiguration();
   //collision dispatcher, there is a parallel version of this somewhere.
   ship_dispatcher = new btCollisionDispatcher(collisionConfiguration);
   //this thing does the collison checking, there is aparallel version somewhere
   ship_solver = new btSequentialImpulseConstraintSolver();

   btVector3 worldMin(-10, 0,-10);
	btVector3 worldMax(10,10,10);
	ship_overlappingPairCache = new btAxisSweep3(worldMin,worldMax);

   //World setup
   ship_world = new btDiscreteDynamicsWorld(ship_dispatcher, ship_overlappingPairCache, ship_solver, ship_collisionConfiguration);
   ship_world->setGravity(btVector3(0, 0, 0));
}

void PhysicsEngine::init() {
   initWorld();
   initShipWorld();
}


void PhysicsEngine::execute(double delta_time) {
   ///-----stepsimulation_start-----
   //cout << "Num of Objects: " << world->getNumCollisionObjects() << endl;
   //cout << "Time: " << delta_time << endl;
   ship_world->stepSimulation(delta_time, 1);

   //world->debugDrawWorld();

   world->stepSimulation(delta_time, 1);

   for (int i = 0; i < components.size(); i++) {
      //cout << "Updating Pcom: " << i << endl;
      static_pointer_cast<PhysicsComponent>(components[i])->updatePosition(ship);
   }

   //print positions of all objects
   /*
   for (int j = world->getNumCollisionObjects() - 1; j >= 0; j--) {
      btCollisionObject* obj = world->getCollisionObjectArray()[j];
      btRigidBody* body = btRigidBody::upcast(obj);
      btTransform trans;
      if (body && body->getMotionState()) {
         body->getMotionState()->getWorldTransform(trans);
      }
      else {
         trans = obj->getWorldTransform();
      }
      //printf("Obj Pos: %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
   }
   for (int j = ship_world->getNumCollisionObjects() - 1; j >= 0; j--) {
      btCollisionObject* obj = ship_world->getCollisionObjectArray()[j];
      btRigidBody* body = btRigidBody::upcast(obj);
      btTransform trans;
      if (body && body->getMotionState()) {
         body->getMotionState()->getWorldTransform(trans);
      }
      else {
         trans = obj->getWorldTransform();
      }
      printf("Obj Pos: %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
   }*/

}
void PhysicsEngine::registerComponent(std::shared_ptr<Component> component) {
   this->components.push_back(component);
   std::shared_ptr<PhysicsComponent> physicsComponent = std::static_pointer_cast<PhysicsComponent>(component);
   collisionShapes.push_back(physicsComponent->get_collisionShape());

   if (physicsComponent->getWorldIndex() == 0) {
      ship_world->addRigidBody(physicsComponent->getBody());
   }
   else {
      world->addRigidBody(physicsComponent->getBody());
   }
}

void PhysicsEngine::removeComponent(PhysicsComponent* component) {

}
