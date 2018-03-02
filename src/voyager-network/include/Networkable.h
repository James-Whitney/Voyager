#pragma once
#ifndef VOYAGER_NETWORKABLE_H_INCLUDED
#define VOYAGER_NETWORKABLE_H_INCLUDED

#include <SFML/Network.hpp>

#include <voyager-core/include/Component.h>
#include <voyager-utils/include/BulletToGlm.h>
#include <bullet/src/btBulletDynamicsCommon.h>
#include <voyager-physics/include/PhysicsComponent.h>

#include <vector>

class Networkable : public Component {
public:
   bool updateThis;

   void init();
   void update(double delta_time);

   sf::Packet packTransform(sf::Packet packet, std::shared_ptr<btTransform> trans);
   void unpackTransform(sf::Packet packet, std::shared_ptr<btTransform> trans);
};

#endif
