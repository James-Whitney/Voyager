#include "include/VFCbox.h"

VFCbox::VFCbox(vec3 center, vec3 min, vec3 max, float radius, std::vector<int> *idx) {
   this->center = center;
   this->min = min;
   this->max = max;
   this->radius = radius;
   for (auto &i : *idx) { this->idxs.push_back(i); }
}
