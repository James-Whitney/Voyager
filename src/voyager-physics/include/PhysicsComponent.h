#pragma once
#ifndef _PHYSICSCOMPONENT_H
#define _PHYSICSCOMPONENT_H

#include <glm/glm/gtc/type_ptr.hpp>

#include <voyager-core/include/Component.h>

class PhysicsComponent : public Component {

public:

   void init();
   void update(double delta_time);

private:

   glm::vec3 velocity = glm::vec3(0, 0, 0);

};

#endif
