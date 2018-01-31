#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include <memory>
#include <vector>

#include "Component.h"

class Entity : private std::vector< std::shared_ptr<Component> > {

public:

   Entity();

   virtual void update(double delta_time);

   virtual void add(std::shared_ptr<Component> component);
   virtual bool remove(std::shared_ptr<Component> component);

private:

   long id;

};

#endif
