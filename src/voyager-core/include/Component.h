#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <memory>

class Entity;

class Component {

public:

   Component();

   long getId() { return this->id; }

   std::shared_ptr<Entity> getEntity() { return this->entity; }
   void setEntity(std::shared_ptr<Entity> entity) { this->entity = entity; }

   bool getActive() { return this->active; }
   void setActive(bool s) { this->active = s; }

   virtual void init() = 0;
   virtual void update(double delta_time) = 0;

protected:

   long id;
   std::shared_ptr<Entity> entity;
   bool active;

};

#endif
