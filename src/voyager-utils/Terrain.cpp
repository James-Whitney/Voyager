#include "include/Terrain.h"

void Terrain::createShape(std::string heightmap_path, float max_height, float vertex_spacing) {
   std::shared_ptr<BitMap> heightmap = std::make_shared<BitMap>();
   heightmap->load(heightmap_path);
}