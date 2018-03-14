#pragma once
#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "Shape.h"
#include "BitMap.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <bullet/src/btBulletDynamicsCommon.h>

class Terrain : public Shape {

public:

   void createShape(std::string heightmap_path, float max_height, float vertex_spacing, float texture_scale);

   std::vector<unsigned char> getHeightData();
   btScalar getHeightScale();
   btScalar getVertexSpacing();
   btScalar getMinHeight();
   btScalar getMaxHeight();
   int getMapWidth();
   int getMapLength();

   void setTextureFilename(std::string filename) { this->texture_filename = filename; }
   void setNormalMapFilename(std::string filename) { this->normal_map_filename = filename; }

   std::string getTexture() { return this->texture_filename; }
   std::string getNormalMap() { return this->normal_map_filename; }
   float getTextureScale() { return this->texture_scale; }

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

   float max_height = 0.0f;
   float min_height = 0.0f;
   float vertex_spacing = 0.0f;

   std::string texture_filename;
   std::string normal_map_filename;

   float texture_scale = 1.0f;
};

#endif