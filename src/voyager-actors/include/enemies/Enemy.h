#pragma once
#ifndef _ENEMEY_H
#define _ENEMEY_H

#include <voyager-physics/include/PhysicsComponent.h>

class Enemy {
public:
   virtual void preparePhysicsComponent(std::shared_ptr<Entity> entity);
   std::shared_ptr<PhysicsComponent> physics_component;
};

#endif
