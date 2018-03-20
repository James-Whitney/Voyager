#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

#include <vector>
#include <memory>

#include "Component.h"

class Engine {

public:

   virtual void init() = 0;
   virtual void execute(double delta_time = 0) = 0;

   virtual void removeFlagged() = 0;

   virtual void registerComponent(std::shared_ptr<Component> component);

protected:

   std::vector< std::shared_ptr<Component> > components;

};

#endif
