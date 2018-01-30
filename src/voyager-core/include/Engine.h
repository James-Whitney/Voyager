#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

#include <vector>
#include <memory>

#include "Component.h"

class Engine {

public:

   void registerComponent(std::shared_ptr<Component> component);

private:

   std::vector< std::shared_ptr<Component> > components;

};

#endif
