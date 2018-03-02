#include "include/Camera.h"

#include <iostream>

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

void Camera::update(glm::vec3 position, float delta_dir) {
   double delta_xPos, delta_yPos, curr_xPos, curr_yPos;
   float delta_pitch, delta_yaw;
   int width, height;

   glfwGetFramebufferSize(window->getHandle(), &width, &height);
   glfwGetCursorPos(window->getHandle(), &curr_xPos, &curr_yPos);
   
   delta_xPos = prev_xPos - curr_xPos;
   delta_yPos = prev_yPos - curr_yPos;
   prev_xPos = curr_xPos;
   prev_yPos = curr_yPos;

   delta_pitch = delta_yPos / (height * 2);
   delta_yaw = delta_xPos / width;

   move(delta_pitch, delta_yaw + delta_dir);
   setPosition(position);
   return;
}

void Camera::move(float deltaPitch, float deltaYaw) {

   this->yaw += deltaYaw;
   this->pitch += deltaPitch;
   this->pitch = glm::clamp(this->pitch, -1.5f, 1.5f);

}

void Camera::setView(float aspect, shared_ptr<MatrixStack> P,
      shared_ptr<MatrixStack> V) {

   P->perspective(this->fov_y, aspect, this->z_near, this->z_far);
   V->lookAt(this->pos, this->getLookAt(), this->up);
}

void Camera::dump() {
   vec3 lookAt = this->getLookAt(false);
   cout << "Camera: (" << this->pos.x << ", " << this->pos.y << ", " << this->pos.z << ") => ("
      << lookAt.x << ", " << lookAt.y << ", " << lookAt.z << ")" << endl;
}
