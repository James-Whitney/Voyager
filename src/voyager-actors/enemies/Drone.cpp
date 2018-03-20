#include "../include/enemies/Drone.h"

using namespace std;

Drone::Drone(shared_ptr<Scene> scene, shared_ptr<NavMap> nav_map, shared_ptr<btTransform> trans) :
   Entity(),
   nav_map(nav_map)
{
   // transform
   this->setTransform(trans);
   this->setScale(make_shared<btVector3>(4, 4, 4));

   // renderable
   this->renderable = make_shared<Renderable>();
   this->renderable->setMesh(scene->meshes[1]);
   shared_ptr<Uber> uber = scene->ubers[6];
   assert(uber != nullptr);
   this->renderable->setUber(uber);
   this->renderable->setCullStatus(true);

   // physics
   // TODO: create physics
   // shared_ptr<PhysicsComponent> phys = make_shared<PhysicsComponent>();
   // this->add(static_pointer_cast<Component>(phys));

   // brain
   this->brain = make_shared<BrainComponent>();
}

void Drone::linkComponents() {
   this->add(this->renderable);
   this->add(this->brain);
}
