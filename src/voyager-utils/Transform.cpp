#include "include/Transform.h"

using namespace glm;
using namespace std;

Transform::Transform(glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll) {
   this->pos = pos;
   this->scale = scale;

   this->pitch = pitch;
   this->yaw = yaw;
   this->roll = roll;
}

glm::vec3 Transform::getDirection() {
   return glm::vec3(cos(pitch) * sin(yaw), 
                    sin(pitch), 
                    cos(pitch) * cos(yaw));
}