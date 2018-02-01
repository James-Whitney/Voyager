#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H

#include <memory>

#include <glm/glm/gtc/type_ptr.hpp>

#include <voyager-utils/include/MatrixStack.h>

class Camera {

public:

   glm::vec3 getLookAt(bool relative = true);

   void move(float deltaPitch, float deltaYaw);

   void setView(float aspect, std::shared_ptr<MatrixStack> P,
      std::shared_ptr<MatrixStack> MV);

protected:

   const glm::vec3 up = glm::vec3(0, 1, 0);
   glm::vec3 pos = glm::vec3(0, 0, 0);
   float pitch = 0;
   float yaw = 0;
   float fov_y = 70.0f;
   float z_near = 0.01f;
   float z_far = 100.0f;

};

#endif