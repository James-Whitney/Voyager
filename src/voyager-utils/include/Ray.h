#pragma once
#ifndef _RAY_H
#define _RAY_H

#include <glm/glm/gtc/type_ptr.hpp>

class Ray {

public:

   Ray();
   Ray(glm::vec3 pos, glm::vec3 dir);

   glm::vec3 getPos() { return this->pos; }
   void setPos(glm::vec3 pos) { this->pos = pos; }
   glm::vec3 getDir() { return this->dir; }
   void setDir(glm::vec3 dir) { this->dir = dir; }

private:

   glm::vec3 pos;
   glm::vec3 dir;

};

#endif
