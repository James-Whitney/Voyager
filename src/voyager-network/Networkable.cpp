#include "include/Networkable.h"
#include <voyager-core/include/Entity.h>

void Networkable::init() {
   updateThis = false;
   prevTransform = this->getEntity()->getTransform();
}

void Networkable::update(double delta_time) {
   std::shared_ptr<Transform> curTransform;
   if (curTransform != prevTransform) {
      updateThis = true;
      prevTransform = curTransform;
   } else {
      updateThis = false;
   }
}
