#pragma once
#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "Shape.h"
#include "BitMap.h"

#include <glm/glm/gtc/type_ptr.hpp>

class Terrain : public Shape {

public:
   
   void createShape(std::string heightmap_path, float max_height, float vertex_spacing);

private:

   struct vertex {
      glm::vec3 pos;
      glm::vec3 normal;
   };

   struct triangle {
      glm::ivec2 v1_idx; // Index into verticies 2D vector
      glm::ivec2 v2_idx;
      glm::ivec2 v3_idx;
      glm::vec3 normal;
   };

   std::shared_ptr<BitMap> heightmap = std::make_shared<BitMap>();
   std::vector<std::vector<vertex>> verticies;
   std::vector<triangle> faces;

   void generateVerticies(float max_height, float vertex_spacing);
   void generateFaces();
   void computeNormals();

   void buildElementBuffer();
   void buildPositionBuffer();
   void buildNormalBuffer();
   void buildTextureBuffer();

};

#endif