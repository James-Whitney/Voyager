#include "../include/enemies/Spawner.h"

#include <cstdlib>
#include <ctime>

using namespace std;

void Spawner::init() {
   srand(time(0));
}

void Spawner::update(double delta_time) {
   // cout << "UPDATING SPAWNER" << endl;
   this->timer += delta_time;
   if (this->timer > this->freq) {
      this->timer = 0;
      this->spawnDrone();
   }
}

void Spawner::spawnDrone() {
   assert(this->app != nullptr);

   // pick the spawn point
   int idx = (rand() / RAND_MAX) * this->size();
   assert((idx >= 0 && idx < this->size()));
   btVector3 loc = this->at(idx);

   // make a drone
   shared_ptr<btTransform> trans = make_shared<btTransform>();
   trans->setOrigin(loc);
   trans->setRotation(btQuaternion(btVector3(0, 0, 1), 0));
   assert(this->scene != nullptr);
   assert(this->nav_map != nullptr);
   shared_ptr<Drone> drone = make_shared<Drone>(this->scene, this->nav_map, trans, this->app);
   drone->setHealth(300.0f);
   drone->initPhysics();
   drone->linkComponents();

   // put the drone in the application
   app->getThings()[drone->getId()] = drone;
   for (int i = 0; i < drone->numComponents(); ++i) {
      auto c = drone->componentAt(i);
      c->init();
      if (dynamic_pointer_cast<Renderable>(c)) {
         app->getRenderEngine()->registerComponent(c);
      } else if (dynamic_pointer_cast<PhysicsComponent>(c)) {
         app->getPhysicsEngine()->registerComponent(c);
      } else if (dynamic_pointer_cast<BrainComponent>(c)) {
         app->getAiEngine()->registerComponent(c);
      }
   }
}
