#pragma once
#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <memory>

#include <glm/glm/gtc/type_ptr.hpp>

#include "Ray.h"

class Transform {

public:

   Transform();
   Transform(glm::vec3 pos, glm::vec3 dir, float scale);
   Transform(std::shared_ptr<Ray> orientation, float scale);

   std::shared_ptr<Ray> getOrientation() { return this->orientation; }
   void setOrientation(std::shared_ptr<Ray> orientation) {
      this->orientation = orientation;
   }

   float getScale() { return this->scale; }
   void setScale(float scale) { this->scale = scale; }

protected:

   std::shared_ptr<Ray> orientation;
   float scale;

};

#endif
