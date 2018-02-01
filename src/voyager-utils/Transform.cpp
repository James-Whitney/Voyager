#include "include/Transform.h"

using namespace glm;
using namespace std;

Transform::Transform(glm::vec3 pos, glm::vec3 dir, float scale) :
   scale(scale)
{
   this->orientation = make_shared<Ray>(pos, dir);
}

Transform::Transform() {

}

Transform::Transform(std::shared_ptr<Ray> orientation, float scale) :
   orientation(orientation),
   scale(scale)
{

}

