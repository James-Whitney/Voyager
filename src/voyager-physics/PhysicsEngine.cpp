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

std::shared_ptr<Entity> PhysicsEngine::findEntityFromBody(const btCollisionObject* ob) {
   for (auto &component: components) {
      if (static_pointer_cast<PhysicsComponent>(component)->getBody() == ob) {
         return static_pointer_cast<PhysicsComponent>(component)->getEntity();
      }
   }
   return nullptr;
}

void PhysicsEngine::clearEntityCollides() {
   for (auto &component: components) {
      static_pointer_cast<PhysicsComponent>(component)->getEntity()->resetCollide();
   }
}

void PhysicsEngine::checkCollision(btDiscreteDynamicsWorld *this_world) {
   int numManifolds = this_world->getDispatcher()->getNumManifolds();
   //For each contact manifold
   for (int i = 0; i < numManifolds; i++) {
      btPersistentManifold* contactManifold =  this_world->getDispatcher()->getManifoldByIndexInternal(i);
      std::shared_ptr<Entity> entityA = findEntityFromBody(contactManifold->getBody0());
      std::shared_ptr<Entity> entityB = findEntityFromBody(contactManifold->getBody1());
      if (entityA == nullptr | entityB == nullptr) {
         printf("ROUGE BODY HAS NO ENTITY! WTF DID YOU DO!\n");
      }
      else {
         entityA->collide(entityB);
         entityB->collide(entityA);
      }
   }
}


void PhysicsEngine::removeFlagged()
{
   for (int i = 0; i < components.size(); i++) {
      std::shared_ptr<PhysicsComponent> component = std::static_pointer_cast<PhysicsComponent>(components[i]);
      if (component->getRemoveFlag()) {
         if (component->getWorldIndex() == 0) {
            ship_world->removeRigidBody(component->getBody());
         }
         else if (component->getWorldIndex() == 1) {
            world->removeRigidBody(component->getBody());
         }
         components.erase(components.begin() + i);
      }
   }
}


void PhysicsEngine::execute(double delta_time) {
   ///-----stepsimulation_start-----
   //cout << "Num of Objects: " << world->getNumCollisionObjects() << endl;
   //cout << "Time: " << delta_time << endl;
   ship_world->stepSimulation(delta_time, 1);
   checkCollision(ship_world);

   world->stepSimulation(delta_time, 1);
   checkCollision(world);

   for (int i = 0; i < components.size(); i++) {
      //cout << "Updating Pcom: " << i << endl;
      static_pointer_cast<PhysicsComponent>(components[i])->updatePosition(ship);
   }
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
