#include "include/PhysicsEngine.h"

void PhysicsEngine::init() {
   //the broadphase, i dont know what tis thing does
   broadphase = new btDbvtBroadphase();
   //thing that decides how things collides
   collisionConfiguration = new btDefaultCollisionConfiguration();
   //collision dispatcher, there is a parallel version of this somewhere.
   dispatcher = new btCollisionDispatcher(collisionConfiguration);
   //this thing does the collison checking, there is aparallel version somewhere
   solver = new btSequentialImpulseConstraintSolver();

   btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	btAxisSweep3* overlappingPairCache = new btAxisSweep3(worldMin,worldMax);

   //World setup
   world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
   world->setGravity(btVector3(0, 0, 0));
}


void PhysicsEngine::execute(double delta_time) {
   ///-----stepsimulation_start-----
   //cout << "Num of Objects: " << world->getNumCollisionObjects() << endl;
   //cout << "Time: " << delta_time << endl;
   world->stepSimulation(delta_time, 2);

   for (int i = 0; i < components.size(); i++) {
      //cout << "Updating Pcom: " << i << endl;
      components[i]->update(delta_time);
   }
   
   //print positions of all objects
   
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
      printf("Obj Pos: %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
   }

}
void PhysicsEngine::registerComponent(std::shared_ptr<Component> component) {
   this->components.push_back(component);
   std::shared_ptr<PhysicsComponent> physicsComponent = std::static_pointer_cast<PhysicsComponent>(component);
   collisionShapes.push_back(physicsComponent->get_collisionShape());
   world->addRigidBody(physicsComponent->getBody());
}

void PhysicsEngine::removeComponent(PhysicsComponent* component) {
   
}