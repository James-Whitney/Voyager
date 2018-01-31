#pragma once
#ifndef _ENGINE_H
#define _ENGINE_H

#include <vector>
#include <memory>

#include "Component.h"

template <typename C>
class Engine {

public:

   void registerComponent(std::shared_ptr<C> component);

private:

   std::vector< std::shared_ptr<C> > components;

};

#endif
