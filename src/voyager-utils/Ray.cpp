#include "include/Ray.h"

using namespace glm;

Ray::Ray() {}

Ray::Ray(vec3 pos, vec3 dir) :
   pos(pos), dir(dir)
{}
