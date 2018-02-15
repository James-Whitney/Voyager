#include "include/Camera.h"

using namespace glm;
using namespace std;

vec3 Camera::getLookAt(bool relative) {
   glm::vec3 lookAt = glm::vec3(cos(pitch) * sin(yaw),
                                sin(pitch),
                                cos(pitch) * cos(yaw));
   if (relative) {
      lookAt += this->pos;
   }
   return lookAt;
}

void Camera::move(float deltaPitch, float deltaYaw) {

   this->yaw += deltaYaw;
   this->pitch += deltaPitch;
   this->pitch = glm::clamp(this->pitch, -1.5f, 1.5f);

}

void Camera::setView(float aspect, shared_ptr<MatrixStack> P,
      shared_ptr<MatrixStack> MV) {

   P->perspective(this->fov_y, aspect, this->z_near, this->z_far);
   MV->lookAt(this->pos, this->getLookAt(), this->up);
}