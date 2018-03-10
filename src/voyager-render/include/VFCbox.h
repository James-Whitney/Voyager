#pragma once
#ifndef VOYAGER_VFC_BOX_H
#define VOYAGER_VFC_BOX_H

#include <glad/glad.h>
#include <math.h>

using namespace glm;

class VFCbox {
public:
   vec3 center;
   vec3 min;
   vec3 max;
   float radius;
   std::vector<int> idxs;

   VFCbox(vec3 center, vec3 min, vec3 max, float radius, std::vector<int> *idx) {
      this->center = center;
      this->min = min;
      this->max = max;
      this->radius = radius;
      for (auto &i : *idx) { this->idxs.push_back(i); }
   }
};

#endif
