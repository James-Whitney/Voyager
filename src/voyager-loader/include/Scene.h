#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>

#include <voyager-core/include/All.h>
#include <voyager-render/include/Uber.h>
#include <voyager-utils/include/Shape.h>

class Scene {

public:

   std::vector< std::shared_ptr<Entity> > entities;

   std::vector< std::shared_ptr<Component> > components;

   std::vector< std::shared_ptr<Shape> > shapes;

   std::vector< std::shared_ptr<Uber> > ubers;

   void apply(std::shared_ptr<Application> app);

   void dump();

};

#endif
