#pragma once
#ifndef VOYAGER_VFC_BOX_H
#define VOYAGER_VFC_BOX_H

#include <glad/glad.h>
#include <glm/glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>

using namespace glm;

class VFCbox {
public:
   vec3 center;
   vec3 min;
   vec3 max;
   float radius;
   std::vector<int> idxs;

   VFCbox(vec3 center, vec3 min, vec3 max, float radius, std::vector<int> *idx);
};

#endif
