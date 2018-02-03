#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <memory>

#include <glm/glm/gtc/type_ptr.hpp>

//#include "Ray.h"

class Transform {

public:

   Transform(glm::vec3 pos, glm::vec3 scale, float pitch, float yaw, float roll);
   //Transform(std::shared_ptr<Ray> orientation, float scale);

   //std::shared_ptr<Ray> getOrientation() { return this->orientation; }
   //void setOrientation(std::shared_ptr<Ray> orientation) {
   //   this->orientation = orientation;
   //}

   glm::vec3 getScale() { return this->scale; }
   void setScale(glm::vec3 scale) { this->scale = scale; }

   glm::vec3 getPosition() { return this->pos; }
   void setPosition(glm::vec3 pos) { this->pos = pos; }

   glm::vec3 getDirection();

   float getPitch() { return this->pitch; }
   void setPitch(float pitch) { this->pitch = pitch; }

   float getYaw() { return this->yaw; }
   void setYaw(float yaw) { this->yaw = yaw; }

   float getRoll() { return this->roll; }
   void setRoll(float roll) { this->roll = roll; }

protected:
   glm::vec3 scale = glm::vec3(1, 1, 1);
   glm::vec3 pos = glm::vec3(0, 0, 0);

   float pitch = 0.0;
   float yaw = 0.0;
   float roll = 0.0;
};

#endif
