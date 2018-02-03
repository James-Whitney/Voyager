#pragma once
#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include <iostream>
#include <vector>

#include <voyager-core/include/Component.h>
#include <voyager-utils/include/Shape.h>
#include <voyager-utils/include/tiny_obj_loader.h>

#include "Uber.h"

class Renderable : public Component {

public:

   std::shared_ptr<Uber> getUber() { return this->uber; }
   void setUber(std::shared_ptr<Uber> uber) { this->uber = uber; }

   std::shared_ptr<Shape> getShape() { return this->shape; }
   void setShape(std::shared_ptr<Shape> shape) { this->shape = shape; }
   void setShape(std::string obj_path);

   virtual void init();
   virtual void update(double delta_time);

private:

   std::shared_ptr<Uber> uber;
   std::shared_ptr<Shape> shape;

};

#endif
