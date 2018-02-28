#pragma once
#ifndef VOYAGER_VIEWFRUSTUMCULLINGBOX_H
#define VOYAGER_VIEWFRUSTUMCULLINGBOX_H

#include <glad/glad.h>

#include <math.h>

using namespace glm;

class VFCbox {
public:
   vec3 min;
   vec3 max;
   float radius;
   std::vector<int> i;

   VFCbox() { }
};

#endif
