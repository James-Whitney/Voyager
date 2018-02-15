#include "include/Entity.h"

using namespace std;

static long nextId = 0;

Entity::Entity() :
   id(nextId++)
{}

void Entity::update(double delta_time) {
   for (int i = 0; i < this->size(); ++i) {
      this->at(i)->update(delta_time);
   }
}

void Entity::add(shared_ptr<Component> component) {
   this->push_back(component);
   component->setEntity(this->shared_from_this());
}

int Entity::numComponents() {
   return this->size();
}

shared_ptr<Component> Entity::componentAt(int i) {
   return this->at(i);
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
