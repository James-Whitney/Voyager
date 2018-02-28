#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H

#include <memory>

#include <glm/glm/gtc/type_ptr.hpp>

#include <voyager-utils/include/MatrixStack.h>

class Camera {

public:

   glm::vec3 getLookAt(bool relative = true);

   void setPosition(glm::vec3 pos) { this->pos = pos; }
   glm::vec3 getPosition() { return this->pos; }

   void move(float deltaPitch, float deltaYaw);
   void move(float deltaPitch, float deltaYaw, float offset);

   void setView(float aspect, std::shared_ptr<MatrixStack> P,
      std::shared_ptr<MatrixStack> V);


   void dump();

protected:

   const glm::vec3 up = glm::vec3(0, 1, 0);
   glm::vec3 pos = glm::vec3(0, 0, 0);


   float pitch = 0;
   float yaw = 0;
   const float fov_y = 70.0f;
   const float z_near = 0.01f;
   const float z_far = 500.0f;

};

#endif
