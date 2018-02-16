#include "include/Terrain.h"

void Terrain::createShape(std::string heightmap_path, float max_height, float vertex_spacing) {
   // Generate terrain from heightmap
   this->heightmap->load(heightmap_path);
   this->generateVerticies(max_height, vertex_spacing);
   this->generateFaces();
   this->computeNormals();

   // Build the shape buffers
   this->buildPositionBuffer();
   this->buildNormalBuffer();
   this->buildElementBuffer();
}

void Terrain::generateVerticies(float max_height, float vertex_spacing) {
   // Erase existing verticies
   this->verticies.clear();

   // Initialize verticies vector
   vertex initial_value = {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)};
   this->verticies.resize(this->heightmap->width, std::vector<vertex>(this->heightmap->height, initial_value));

   // Generate verticies
   for (int z = 0; z < this->heightmap->height; z++) {
      for (int x = 0; x < this->heightmap->width; x++) {
         // Get heightmap pixel
         BitMap::pixel p = this->heightmap->getPixel(x, z);

         // Normalize grayscale value to 0 to 1 range
         float normalized = p.r / 255.0f;

         // Build vertex
         vertex v;
         v.pos.x = x * vertex_spacing;
         v.pos.y = normalized * max_height;
         v.pos.z = z * vertex_spacing;
         v.normal = glm::vec3(0.0f, 1.0f, 0.0f);

         this->verticies[x][z] = v;
      }
   }
}

void Terrain::generateFaces() {
   // Erase existing faces
   this->faces.clear();

   // Generate faces
   for (int z = 0; z < this->heightmap->height - 1; z++) {
      for (int x = 0; x < this->heightmap->width - 1; x++) {
         triangle t1;
         t1.v1_idx = glm::ivec2(x, z);
         t1.v2_idx = glm::ivec2(x, z + 1);
         t1.v3_idx = glm::ivec2(x + 1, z + 1);

         triangle t2;
         t2.v1_idx = glm::ivec2(x, z);
         t2.v2_idx = glm::ivec2(x + 1, z + 1);
         t2.v3_idx = glm::ivec2(x + 1, z);

         this->faces.push_back(t1);
         this->faces.push_back(t2);
      }
   }
}

void Terrain::computeNormals() {
   // Erase existing vertex normals
   for (int z = 0; z < this->heightmap->height; z++) {
      for (int x = 0; x < this->heightmap->width; x++) {
         this->verticies[x][z].normal = glm::vec3(0.0f, 0.0f, 0.0f);
      }
   }

   // Calculate surface normals and nonnormalized vertex normals
   for (int i = 0; i < this->faces.size(); i++) {
      triangle t = this->faces[i];
      vertex *v1 = &verticies[t.v1_idx.x][t.v1_idx.y];
      vertex *v2 = &verticies[t.v2_idx.x][t.v2_idx.y];
      vertex *v3 = &verticies[t.v3_idx.x][t.v3_idx.y];

      // Calculate surface normal
      glm::vec3 edge1 = v2->pos - v1->pos;
      glm::vec3 edge2 = v3->pos - v1->pos;

      glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
      this->faces[i].normal = normal;

      // Add surface normal to vertex normal
      v1->normal += normal;
      v2->normal += normal;
      v3->normal += normal;
   }

   // Normalize vertex normals
   for (int z = 0; z < this->heightmap->height; z++) {
      for (int x = 0; x < this->heightmap->width; x++) {
         this->verticies[x][z].normal = glm::normalize(this->verticies[x][z].normal);
      }
   }
}

void Terrain::buildElementBuffer() {
   // Clear existing element buffer
   this->eleBuf.clear();

   // Build the element buffer
   for (int i = 0; i < this->faces.size(); i++) {
      triangle t = this->faces[i];
      this->eleBuf.push_back(t.v1_idx.y * this->heightmap->width + t.v1_idx.x);
      this->eleBuf.push_back(t.v2_idx.y * this->heightmap->width + t.v2_idx.x);
      this->eleBuf.push_back(t.v3_idx.y * this->heightmap->width + t.v3_idx.x);
   }
}

void Terrain::buildPositionBuffer() {
   // Clear existing position buffer
   this->posBuf.clear();

   // Build the position buffer
   for (int z = 0; z < this->heightmap->height; z++) {
      for (int x = 0; x < this->heightmap->width; x++) {
         vertex v = this->verticies[x][z];
         this->posBuf.push_back(v.pos.x);
         this->posBuf.push_back(v.pos.y);
         this->posBuf.push_back(v.pos.z);
      }
   }
}

void Terrain::buildNormalBuffer() {
   // Clear existing normal buffer
   this->norBuf.clear();

   // Build the normal buffer
   for (int z = 0; z < this->heightmap->height; z++) {
      for (int x = 0; x < this->heightmap->width; x++) {
         vertex v = this->verticies[x][z];
         this->norBuf.push_back(v.normal.x);
         this->norBuf.push_back(v.normal.y);
         this->norBuf.push_back(v.normal.z);
      }
   }
}