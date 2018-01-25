#pragma once
#ifndef _RAY_H
#define _RAY_H

#include <glm/gtc/type_ptr.hpp>

class Ray {
public:
   Ray();
   Ray(glm::vec3 pos, glm::vec3 dir);

   glm::vec3 pos;
   glm::vec3 dir;
};

#endif
