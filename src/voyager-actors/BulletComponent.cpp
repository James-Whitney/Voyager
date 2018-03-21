#include "include/BulletComponent.h"

void BulletComponent::init() {

}

void BulletComponent::update(double delta_time)
{
   logicUpdate(delta_time);
   return;
}

void BulletComponent::removeBullet()
{
   for (int i = 0; i < getEntity()->numComponents(); i++) {
      getEntity()->componentAt(i)->setRemoveFlag();
   }
   for (int i = 0; i < getEntity()->numComponents(); i++) {
      getEntity()->removeComponentAt(i);
   }
   app->getThings().erase(getEntity()->getId());
}

void BulletComponent::logicUpdate(double delta_time)
{
   life_time += delta_time;
   if (life_time > 300.0) {
   // if (life_time > 300.0 || getEntity()->getCollideList().size() > 0) {
      removeBullet();
   } 
   return;
}
