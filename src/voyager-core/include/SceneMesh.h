#pragma once
#ifndef _SCENEMESH_H
#define _SCENEMESH_H

#include <vector>
#include <voyager-utils/include/Shape.h>

class SceneMesh {
   public:
      std::vector< std::vector< std::shared_ptr<Shape> > > meshes;
      std::vector< std::shared_ptr<Uber> > ubers;
};

#endif
