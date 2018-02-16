#pragma once
#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "Shape.h"
#include "BitMap.h"

class Terrain : public Shape {

public:
   
   void createShape(std::string heightmap_path, float max_height, float vertex_spacing);

};

#endif