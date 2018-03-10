#include "include/Skybox.h"

Skybox::Skybox(std::string top, std::string bottom, std::string front, std::string back, std::string left, std::string right) :
   top(top),
   bottom(bottom),
   front(front),
   back(back),
   left(left),
   right(right)
{
}

void Skybox::init() {
   this->initGeometry();
   this->initTextures();
}

void Skybox::initGeometry()
{
   float boxSize = 1.0f;
   GLfloat skyboxVertices[] = {
      -boxSize, boxSize, -boxSize,
      -boxSize, -boxSize, -boxSize,
      boxSize, -boxSize, -boxSize,
      boxSize, -boxSize, -boxSize,
      boxSize, boxSize, -boxSize,
      -boxSize, boxSize, -boxSize,

      -boxSize, -boxSize, boxSize,
      -boxSize, -boxSize, -boxSize,
      -boxSize, boxSize, -boxSize,
      -boxSize, boxSize, -boxSize,
      -boxSize, boxSize, boxSize,
      -boxSize, -boxSize, boxSize,

      boxSize, -boxSize, -boxSize,
      boxSize, -boxSize, boxSize,
      boxSize, boxSize, boxSize,
      boxSize, boxSize, boxSize,
      boxSize, boxSize, -boxSize,
      boxSize, -boxSize, -boxSize,

      -boxSize, -boxSize, boxSize,
      -boxSize, boxSize, boxSize,
      boxSize, boxSize, boxSize,
      boxSize, boxSize, boxSize,
      boxSize, -boxSize, boxSize,
      -boxSize, -boxSize, boxSize,

      -boxSize, boxSize, -boxSize,
      boxSize, boxSize, -boxSize,
      boxSize, boxSize, boxSize,
      boxSize, boxSize, boxSize,
      -boxSize, boxSize, boxSize,
      -boxSize, boxSize, -boxSize,

      -boxSize, -boxSize, -boxSize,
      -boxSize, -boxSize, boxSize,
      boxSize, -boxSize, -boxSize,
      boxSize, -boxSize, -boxSize,
      -boxSize, -boxSize, boxSize,
      boxSize, -boxSize, boxSize
   };

   // Generate the VAO
   CHECKED_GL_CALL(glGenVertexArrays(1, &this->SkyboxVertexArrayID));
   CHECKED_GL_CALL(glBindVertexArray(this->SkyboxVertexArrayID));

   // Generate and bind the vertex buffer
   CHECKED_GL_CALL(glGenBuffers(1, &this->skyboxvertexbuffer));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->skyboxvertexbuffer));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_DYNAMIC_DRAW));
   CHECKED_GL_CALL(glEnableVertexAttribArray(0));
   CHECKED_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0));
}

void Skybox::initTextures() {
   // Generate texture bindings
   CHECKED_GL_CALL(glGenTextures(1, &this->textureID));
   CHECKED_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID));

   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0));
   CHECKED_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0));

   int width, height, channels;

   // Right texture
   unsigned char *tex = stbi_load(this->right.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);

   // Left texture
   tex = stbi_load(this->left.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);

   // Top texture
   tex = stbi_load(this->top.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);

   // Bottom texture
   tex = stbi_load(this->bottom.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);

   // Back texture
   tex = stbi_load(this->back.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);

   // Front texture
   tex = stbi_load(this->front.c_str(), &width, &height, &channels, 0);
   CHECKED_GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex));
   stbi_image_free(tex);
}

void Skybox::draw() {
   CHECKED_GL_CALL(glDepthMask(GL_FALSE));

   CHECKED_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

   // Bind the texture
   CHECKED_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID));

   // Bind vertex array object
   CHECKED_GL_CALL(glBindVertexArray(this->SkyboxVertexArrayID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->skyboxvertexbuffer));
   CHECKED_GL_CALL(glEnableVertexAttribArray(0));
   CHECKED_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0));

   // Draw the skybox
   CHECKED_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 36));

   CHECKED_GL_CALL(glDepthMask(GL_TRUE));
}