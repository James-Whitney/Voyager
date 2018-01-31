#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>

#include "Component.h"

class Entity : private std::vector< std::shared_ptr<Component> > {

public:

   Entity();

   long getId() { return this->id; }

   virtual void update(double delta_time);

   virtual void add(std::shared_ptr<Component> component);
   virtual std::shared_ptr<Component> componentAt(int i);
   virtual bool remove(std::shared_ptr<Component> component);

protected:

   long id;

};

#endif
