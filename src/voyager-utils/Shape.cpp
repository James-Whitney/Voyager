#include "include/Shape.h"

#include <iostream>
#include <cassert>

#include "include/GLSL.h"
#include "include/Program.h"

using namespace std;

// copy the data from the shape to this object
void Shape::createShape(tinyobj::shape_t &shape)
{
   posBuf = shape.mesh.positions;
   norBuf = shape.mesh.normals;
   texBuf = shape.mesh.texcoords;
   eleBuf = shape.mesh.indices;
}

void Shape::measure()
{
   float minX, minY, minZ;
   float maxX, maxY, maxZ;

   minX = minY = minZ = std::numeric_limits<float>::max();
   maxX = maxY = maxZ = -std::numeric_limits<float>::max();

   //Go through all vertices to determine min and max of each dimension
   for (size_t v = 0; v < posBuf.size() / 3; v++)
   {
      if (posBuf[3 * v + 0] < minX)
         minX = posBuf[3 * v + 0];
      if (posBuf[3 * v + 0] > maxX)
         maxX = posBuf[3 * v + 0];

      if (posBuf[3 * v + 1] < minY)
         minY = posBuf[3 * v + 1];
      if (posBuf[3 * v + 1] > maxY)
         maxY = posBuf[3 * v + 1];

      if (posBuf[3 * v + 2] < minZ)
         minZ = posBuf[3 * v + 2];
      if (posBuf[3 * v + 2] > maxZ)
         maxZ = posBuf[3 * v + 2];
   }

   min.x = minX;
   min.y = minY;
   min.z = minZ;
   max.x = maxX;
   max.y = maxY;
   max.z = maxZ;
   radius = (float)sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2))/2.0f;
}

void Shape::init()
{
   // Initialize the vertex array object
   CHECKED_GL_CALL(glGenVertexArrays(1, &vaoID));
   CHECKED_GL_CALL(glBindVertexArray(vaoID));

   // Send the position array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &posBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_STATIC_DRAW));

   // Send the normal array to the GPU
   if (norBuf.empty())
   {
      norBufID = 0;
   }
   else
   {
      CHECKED_GL_CALL(glGenBuffers(1, &norBufID));
      CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
      CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, norBuf.size() * sizeof(float), &norBuf[0], GL_STATIC_DRAW));
   }

   // If we don't have a texture buffer, generate a default one
   if (texBuf.empty())
   {
      texBuf.resize(this->posBuf.size(), 0.0f);
   }

   // Send the texture array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &texBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, texBuf.size() * sizeof(float), &texBuf[0], GL_STATIC_DRAW));

   // Send the element array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &eleBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));
   CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size() * sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW));

   // Calculate tangents and bitangents
   this->calculateTangentsAndBitangents();

   // Send the tangent array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &tanBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, tanBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, tanBuf.size() * sizeof(float), &tanBuf[0], GL_STATIC_DRAW));

   // Send the bitangent array to the GPU
   CHECKED_GL_CALL(glGenBuffers(1, &bitanBufID));
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, bitanBufID));
   CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, bitanBuf.size() * sizeof(float), &bitanBuf[0], GL_STATIC_DRAW));

   // Unbind the arrays
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
   CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Shape::calculateTangentsAndBitangents() {
   // Build a vector of verticies
   std::vector<glm::vec3> verticies;
   for (int i = 0; i < posBuf.size(); i += 3) {
      verticies.push_back(glm::vec3(posBuf[i], posBuf[i + 1], posBuf[i + 2]));
   }

   // Build a vector of texture coordinates
   std::vector<glm::vec2> uv;
   for (int i = 0; i < texBuf.size(); i += 2) {
      uv.push_back(glm::vec2(texBuf[i], texBuf[i + 1]));
   }

   // Initialize the tangent and bitangent vectors
   std::vector<glm::vec3> tangents(verticies.size(), glm::vec3(0.0f, 0.0f, 0.0f));
   std::vector<glm::vec3> bitangents(verticies.size(), glm::vec3(0.0f, 0.0f, 0.0f));

   // For each triangle, calculate the tangent and bitangent
   // Add the result to the respective vertex's other tangents and bitangents
   for (int i = 0; i < eleBuf.size(); i += 3) {
      unsigned int v1 = eleBuf[i];
      unsigned int v2 = eleBuf[i + 1];
      unsigned int v3 = eleBuf[i + 2];

      glm::vec3 pos1 = verticies[v1];
      glm::vec3 pos2 = verticies[v2];
      glm::vec3 pos3 = verticies[v3];

      glm::vec2 uv1 = uv[v1];
      glm::vec2 uv2 = uv[v2];
      glm::vec2 uv3 = uv[v3];

      glm::vec3 edge1 = pos2 - pos1;
      glm::vec3 edge2 = pos3 - pos1;
      glm::vec2 deltaUV1 = uv2 - uv1;
      glm::vec2 deltaUV2 = uv3 - uv1;

      float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

      // Calculate the tangent of the triangle
      glm::vec3 tangent;
      tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
      tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
      tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

      // Calculate the bitangent of the vector
      glm::vec3 bitangent;
      bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
      bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
      bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

      tangents[v1] += tangent;
      tangents[v2] += tangent;
      tangents[v3] += tangent;

      bitangents[v1] += bitangent;
      bitangents[v2] += bitangent;
      bitangents[v3] += bitangent;
   }

   // Erase the existing tangent and bitangent buffers
   this->tanBuf.clear();
   this->bitanBuf.clear();

   // Normalize the tangents and bitangents and fill buffers
   for (int i = 0; i < tangents.size(); i++) {
      glm::vec3 tangent = glm::normalize(tangents[i]);
      this->tanBuf.push_back(tangent.x);
      this->tanBuf.push_back(tangent.y);
      this->tanBuf.push_back(tangent.z);

      glm::vec3 bitangent = glm::normalize(bitangents[i]);
      this->bitanBuf.push_back(bitangent.x);
      this->bitanBuf.push_back(bitangent.y);
      this->bitanBuf.push_back(bitangent.z);
   }
}

void Shape::draw(const shared_ptr<Program> prog) const
{
   int h_pos, h_nor, h_tex, h_tan, h_bitan;
   h_pos = h_nor = h_tex = h_tan = h_bitan = -1;

   CHECKED_GL_CALL(glBindVertexArray(vaoID));

   // Bind position buffer
   h_pos = prog->getAttribute("vertPos");
   GLSL::enableVertexAttribArray(h_pos);
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
   CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

   // Bind normal buffer
   h_nor = prog->getAttribute("vertNor");
   if (h_nor != -1 && norBufID != 0)
   {
      GLSL::enableVertexAttribArray(h_nor);
      CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
      CHECKED_GL_CALL(glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
   }

   if (texBufID != 0)
   {
      // Bind texcoords buffer
      h_tex = prog->getAttribute("vertTex");

      if (h_tex != -1 && texBufID != 0)
      {
         GLSL::enableVertexAttribArray(h_tex);
         CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
         CHECKED_GL_CALL(glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0));
      }
   }

   // Bind tangent buffer
   h_tan = prog->getAttribute("vertTan");
   if (h_tan != -1 && tanBufID != 0)
   {
      GLSL::enableVertexAttribArray(h_tan);
      CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, tanBufID));
      CHECKED_GL_CALL(glVertexAttribPointer(h_tan, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
   }

   // Bind bitangent buffer
   h_bitan = prog->getAttribute("vertBitan");
   if (h_bitan != -1 && bitanBufID != 0)
   {
      GLSL::enableVertexAttribArray(h_bitan);
      CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, bitanBufID));
      CHECKED_GL_CALL(glVertexAttribPointer(h_bitan, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
   }

   // Bind element buffer
   CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));

   // Draw
   CHECKED_GL_CALL(glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0));

   // Disable and unbind
   if (h_tex != -1)
   {
      GLSL::disableVertexAttribArray(h_tex);
   }
   if (h_nor != -1)
   {
      GLSL::disableVertexAttribArray(h_nor);
   }
   if (h_tan != -1)
   {
      GLSL::disableVertexAttribArray(h_tan);
   }
   if (h_bitan != -1)
   {
      GLSL::disableVertexAttribArray(h_bitan);
   }
   GLSL::disableVertexAttribArray(h_pos);
   CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
   CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
