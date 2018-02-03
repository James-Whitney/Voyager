#pragma once
#ifndef VOYAGER_NETWORKABLE_H_INCLUDED
#define VOYAGER_NETWORKABLE_H_INCLUDED

#include <SFML/Network.hpp>

#include <voyager-core/include/Component.h>

#include <vector>

class Networkable : public Component {
public:
   void init() { }
   void update(double delta_time) { }
};

#endif
