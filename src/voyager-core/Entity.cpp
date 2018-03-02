#include "include/Entity.h"

using namespace std;

static long nextId = 0;

Entity::Entity() :
   id(nextId++)
{}

shared_ptr<btTransform> Entity::getTransform(bool absolute) {
   if (absolute && this->parent != nullptr) {
      shared_ptr<btTransform> parent_transform = this->getParent()->getTransform(true);
      // TODO: concatenate transforms
      return this->transform;
   } else {
      return this->transform;
   }
}

child_iter Entity::startChild() {
   return this->children.begin();
}

child_iter Entity::endChild() {
   return this->children.end();
}

void Entity::addChild(shared_ptr<Entity> child) {
   this->children.push_back(child);
}

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
