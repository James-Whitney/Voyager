#include "include/Entity.h"

using namespace std;

void Entity::update(double delta_time) {
   for (int i = 0; i < this->size(); ++i) {
      this->at(i)->update(delta_time);
   }
}

void Entity::add(shared_ptr<Component> component) {
   this->push_back(component);
}

bool Entity::remove(shared_ptr<Component> component) {
   for (int i = 0; i < this->size(); ++i) {
      if (this->at(i).get() == component.get()) {
         this->erase(this->begin() + i);
         return true;
      }
   }
   return false;
}
