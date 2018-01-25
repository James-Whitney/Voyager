#include "Ray.h"

using namespace glm;

Ray::Ray() :
   pos(0, 0, 0),
   dir(1, 0, 0)
{
}

Ray::Ray(vec3 pos, vec3 dir) :
   pos(pos),
   dir(dir)
{
}
