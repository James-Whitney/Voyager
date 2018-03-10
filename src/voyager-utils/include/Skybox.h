#pragma once
#ifndef _SKYBOX_H
#define _SKYBOX_H

#include <stb/stb_image.h>

#include "Texture.h"

#include <iostream>
#include "GLSL.h"

#include <glm/glm/gtc/type_ptr.hpp>

class Skybox {

public:

   typedef struct {
      glm::vec3 color;
      float density;
      float height;
   } Fog;

   Skybox(std::string top, std::string bottom, std::string front, std::string back, std::string left, std::string right);

   void init();
   void draw();

   Fog fog;

private:

   void initGeometry();
   void initTextures();

   std::string top;
   std::string bottom;
   std::string front;
   std::string back;
   std::string left;
   std::string right;

   GLuint SkyboxVertexArrayID;
   GLuint skyboxvertexbuffer;
   unsigned int textureID;

};

#endif