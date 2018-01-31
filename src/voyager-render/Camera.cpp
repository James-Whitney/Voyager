#include "include/Camera.h"

using namespace glm;
using namespace std;

vec3 Camera::getLookAt(bool relative) {

   vec3 lookAt = vec3(
      cos(this->pitch) * cos(this->yaw),
      sin(this->pitch),
      cos(this->pitch) * cos(pi<float>() / 2.0 - this->yaw)
   );

   if (relative) {
      lookAt += this->pos;
   }

   return lookAt;

}

void Camera::move(float deltaPitch, float deltaYaw) {

   this->yaw += deltaYaw;
   this->pitch -= deltaPitch;
   this->pitch = glm::clamp(this->pitch, -0.95f, 0.95f);

}

void Camera::setView(float aspect, shared_ptr<MatrixStack> P,
      shared_ptr<MatrixStack> MV) {

   P->perspective(this->fov_y, aspect, this->z_near, this->z_far);
   MV->lookAt(this->pos, this->getLookAt(), this->up);

}
