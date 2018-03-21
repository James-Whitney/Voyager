#include "include/ActorEngine.h"

void ActorEngine::init() {

}

void ActorEngine::removeFlagged()
{
   for (int i = 0; i < components.size(); i++) {
      std::shared_ptr<Component> component = components[i];
      if (component->getRemoveFlag()) {
         components.erase(components.begin() + i);
      }
   }
}

void ActorEngine::execute(double delta_time) {
   for (int i = 0; i < components.size(); i++) {
      components[i]->update(delta_time);
   }
}