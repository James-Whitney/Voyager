#include "include/DebugBox.h"

DebugBox::DebugBox(btVector3 btAabbMin, btVector3 btAabbMax) {
   btAabbMin = btVector3(btAabbMin.getX()/2.0f,btAabbMin.getY()/2.0f,btAabbMin.getZ()/2.0f); 
   btAabbMax = btVector3(btAabbMax.getX()/2.0f,btAabbMax.getY()/2.0f,btAabbMax.getZ()/2.0f); 

   this->aabbMin = glm::vec3((float)btAabbMin[0], (float)btAabbMin[1], (float)btAabbMin[2]);
   this->aabbMax = glm::vec3((float)btAabbMax[0], (float)btAabbMax[1], (float)btAabbMax[2]);
}

void DebugBox::init() {
   // Generate the VAO
   CHECKED_GL_CALL(glGenVertexArrays(1, &this->vaoID));
   CHECKED_GL_CALL(glBindVertexArray(this->vaoID));

   this->buildPositionBuffer(aabbMin, aabbMax);
   this->buildNormalBuffer(aabbMin, aabbMax);

   // Unbind the arrays
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void DebugBox::buildPositionBuffer(glm::vec3 min, glm::vec3 max) {
   GLfloat verticies[] = {
      min.x, min.y, min.z,
      min.x, max.y, min.z,
      max.x, max.y, min.z,
      min.x, min.y, min.z,
      max.x, max.y, min.z,
      max.x, min.y, min.z,

      min.x, min.y, max.z,
      min.x, max.y, max.z,
      min.x, max.y, min.z,
      min.x, min.y, max.z,
      min.x, max.y, min.z,
      min.x, min.y, min.z,

      min.x, max.y, min.z,
      min.x, max.y, max.z,
      max.x, max.y, max.z,
      min.x, max.y, min.z,
      max.x, max.y, max.z,
      max.x, max.y, min.z,

      max.x, min.y, min.z,
      max.x, max.y, min.z,
      max.x, max.y, max.z,
      max.x, min.y, min.z,
      max.x, max.y, max.z,
      max.x, min.y, max.z,

      min.x, min.y, max.z,
      min.x, min.y, min.z,
      max.x, min.y, min.z,
      min.x, min.y, max.z,
      max.x, min.y, min.z,
      max.x, min.y, max.z,

      max.x, min.y, max.z,
      max.x, max.y, max.z,
      min.x, max.y, max.z,
      max.x, min.y, max.z,
      min.x, max.y, max.z,
      min.x, min.y, max.z
   };

   // Send the position array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &this->posBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->posBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW));
}

void DebugBox::buildNormalBuffer(glm::vec3 min, glm::vec3 max) {
   // Face 0 normal
   glm::vec3 e1 = glm::vec3(min.x, max.y, min.z) - glm::vec3(min.x, min.y, min.z);
   glm::vec3 e2 = glm::vec3(max.x, max.y, min.z) - glm::vec3(min.x, min.y, min.z);
   glm::vec3 n0 = glm::normalize(glm::cross(e1, e2));

   // Face 1 normal
   e1 = glm::vec3(min.x, max.y, max.z) - glm::vec3(min.x, min.y, max.z);
   e2 = glm::vec3(min.x, max.y, min.z) - glm::vec3(min.x, min.y, max.z);
   glm::vec3 n1 = glm::normalize(glm::cross(e1, e2));

   // Face 2 normal
   e1 = glm::vec3(min.x, max.y, max.z) - glm::vec3(min.x, max.y, min.z);
   e2 = glm::vec3(max.x, max.y, max.z) - glm::vec3(min.x, max.y, min.z);
   glm::vec3 n2 = glm::normalize(glm::cross(e1, e2));

   // Face 3 normal
   e1 = glm::vec3(max.x, max.y, min.z) - glm::vec3(max.x, min.y, min.z);
   e2 = glm::vec3(max.x, max.y, max.z) - glm::vec3(max.x, min.y, min.z);
   glm::vec3 n3 = glm::normalize(glm::cross(e1, e2));

   // Face 4 normal
   e1 = glm::vec3(min.x, min.y, min.z) - glm::vec3(min.x, min.y, max.z);
   e2 = glm::vec3(max.x, min.y, max.z) - glm::vec3(min.x, min.y, max.z);
   glm::vec3 n4 = glm::normalize(glm::cross(e1, e2));

   // Face 5 normal
   e1 = glm::vec3(max.x, max.y, max.z) - glm::vec3(max.x, min.y, min.z);
   e2 = glm::vec3(min.x, max.y, max.z) - glm::vec3(max.x, min.y, min.z);
   glm::vec3 n5 = glm::normalize(glm::cross(e1, e2));

   GLfloat normals[] = {
      n0.x, n0.y, n0.z,
      n0.x, n0.y, n0.z,
      n0.x, n0.y, n0.z,
      n0.x, n0.y, n0.z,
      n0.x, n0.y, n0.z,
      n0.x, n0.y, n0.z,

      n1.x, n1.y, n1.z,
      n1.x, n1.y, n1.z,
      n1.x, n1.y, n1.z,
      n1.x, n1.y, n1.z,
      n1.x, n1.y, n1.z,
      n1.x, n1.y, n1.z,

      n2.x, n2.y, n2.z,
      n2.x, n2.y, n2.z,
      n2.x, n2.y, n2.z,
      n2.x, n2.y, n2.z,
      n2.x, n2.y, n2.z,
      n2.x, n2.y, n2.z,

      n3.x, n3.y, n3.z,
      n3.x, n3.y, n3.z,
      n3.x, n3.y, n3.z,
      n3.x, n3.y, n3.z,
      n3.x, n3.y, n3.z,
      n3.x, n3.y, n3.z,

      n4.x, n4.y, n4.z,
      n4.x, n4.y, n4.z,
      n4.x, n4.y, n4.z,
      n4.x, n4.y, n4.z,
      n4.x, n4.y, n4.z,
      n4.x, n4.y, n4.z,

      n5.x, n5.y, n5.z,
      n5.x, n5.y, n5.z,
      n5.x, n5.y, n5.z,
      n5.x, n5.y, n5.z,
      n5.x, n5.y, n5.z,
      n5.x, n5.y, n5.z
   };

   // Send the normal array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &this->norBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->norBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW));
}

void DebugBox::draw(std::shared_ptr<Program> prog, bool wireframe) {
   glUniform1ui(prog->getUniform("uberMode"), UberMode::UBER_MODE_NORMAL);

   CHECKED_GL_CALL(glBindVertexArray(this->vaoID));

   int h_pos = prog->getAttribute("vertPos");
   GLSL::enableVertexAttribArray(h_pos);
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->posBufID));
   CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

   int h_nor = prog->getAttribute("vertNor");
   GLSL::enableVertexAttribArray(h_nor);
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, this->norBufID));
   CHECKED_GL_CALL(glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

   if (wireframe) {
      CHECKED_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
   }

   CHECKED_GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 12 * 3));
   CHECKED_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

   GLSL::disableVertexAttribArray(h_pos);
   GLSL::disableVertexAttribArray(h_nor);
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}