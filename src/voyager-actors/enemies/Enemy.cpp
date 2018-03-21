#include "../include/enemies/Enemy.h"

using namespace std;

void Enemy::preparePhysicsComponent(shared_ptr<Entity> entity) {
   bool found = false;
   for (int i = 0; i < entity->numComponents(); ++i) {
      auto component = entity->componentAt(i);
      if (dynamic_pointer_cast<PhysicsComponent>(component)) {
         assert(!found); // already found one...
         this->physics_component = static_pointer_cast<PhysicsComponent>(component);
         found = true;
      }
   }
   assert(found); // one must be found
}
