#pragma once
#ifndef VOYAGER_NETWORKABLE_H_INCLUDED
#define VOYAGER_NETWORKABLE_H_INCLUDED

#include <SFML/Network.hpp>

#include <voyager-core/include/Component.h>
#include <voyager-utils/include/Transform.h>

#include <vector>

class Networkable : public Component {
public:
   bool updateThis;
   std::shared_ptr<Transform> prevTransform;

   void init();
   void update(double delta_time);
};

#endif
