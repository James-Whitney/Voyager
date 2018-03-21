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

   std::vector<std::shared_ptr<Shape>> getMesh() { return this->mesh; }
   void setMesh(std::vector<std::shared_ptr<Shape>> mesh) { this->mesh = mesh; }

   bool getCullStatus() { return this->shouldCull; }
   void setCullStatus(bool status) { this->shouldCull = status; }

   virtual void init();
   virtual void renderableInit(std::string resource_dir);
   virtual bool hasCustomProgram() { return false; };
   virtual void update(double delta_time);
   virtual void draw(std::shared_ptr<Program> prog);

private:

   std::shared_ptr<Uber> uber;
   std::vector<std::shared_ptr<Shape>> mesh;
   bool shouldCull = false;

};

#endif
