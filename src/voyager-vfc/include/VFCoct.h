#pragma once
#ifndef VOYAGER_VFC_OCT_H
#define VOYAGER_VFC_OCT_H

#include <glad/glad.h>
#include <math.h>
#include <limits>

#include "VFCbox.h"

using namespace glm;

class VFCoct {
public:
   std::shared_ptr<VFCoct> children[8];
   std::vector< std::shared_ptr<VFCbox> > objs;
   VFCbox *box;

   VFCoct(std::vector< std::shared_ptr<VFCbox> > box);
   void generateBox();
   void generateChildren();
};

#endif
