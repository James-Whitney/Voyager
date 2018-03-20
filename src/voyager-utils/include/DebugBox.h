#pragma once
#ifndef _DEBUGBOX_H
#define _DEBUGBOX_H

#include <iostream>

#include <voyager-core/include/Entity.h>
#include <voyager-render/include/Uber.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include <bullet/src/btBulletDynamicsCommon.h>

#include "GLSL.h"
#include "Program.h"

class DebugBox {

public:

   DebugBox(btVector3 btAabbMin, btVector3 btAabbMax);

   void init();
   void draw(std::shared_ptr<Program> prog, bool wireframe);

   void setTransform(std::shared_ptr<btTransform> transform) { this->transform = transform; }
   void setScale(std::shared_ptr<btVector3> scale) { this->scale = scale; }
   void setEntity(std::shared_ptr<Entity> entity) { this->entity = entity; }

   std::shared_ptr<btTransform> getTransform() { return this->transform; }
   std::shared_ptr<btVector3> getScale() { return this->scale; }
   std::shared_ptr<Entity> getEntity() { return this->entity; }

private:

   void buildPositionBuffer(glm::vec3 min, glm::vec3 max);
   void buildNormalBuffer(glm::vec3 min, glm::vec3 max);

   GLuint vaoID;
   GLuint posBufID;
   GLuint norBufID;

   glm::vec3 aabbMin;
   glm::vec3 aabbMax;

   std::shared_ptr<btTransform> transform = nullptr;
   std::shared_ptr<btVector3> scale = nullptr;
   std::shared_ptr<Entity> entity = nullptr;

};

#endif