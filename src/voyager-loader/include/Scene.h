#pragma once
#ifndef _SCENE_H
#define _SCENE_H

#include <vector>

#include <voyager-core/include/All.h>

#include <voyager-render/include/Uber.h>
#include <voyager-render/include/Renderable.h>
#include <voyager-render/include/RenderEngine.h>

#include <voyager-utils/include/Terrain.h>

#include <voyager-physics/include/PhysicsComponent.h>
#include <voyager-physics/include/PhysicsEngine.h>

#include <voyager-actors/include/PlayerComponent.h>
#include <voyager-actors/include/ShipComponent.h>


#include <iostream>



class Scene {

public:

   std::vector< std::shared_ptr<Entity> > entities;

   std::vector< std::shared_ptr<Component> > components;

   std::vector< std::shared_ptr<Shape> > shapes;

   std::vector< std::shared_ptr<Uber> > ubers;

   void initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain);

   void apply(std::shared_ptr<Application> app);

   void dump();

};

#endif
