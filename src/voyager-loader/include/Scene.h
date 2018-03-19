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

#include <voyager-actors/include/ActorEngine.h>
#include <voyager-actors/include/PlayerComponent.h>
#include <voyager-actors/include/ShipComponent.h>
#include <voyager-actors/include/HelmComponent.h>
#include <voyager-actors/include/TurretComponent.h>
#include <voyager-actors/include/ai/NavMapEntity.h>

#include <iostream>

class Scene {

public:

   std::vector< std::shared_ptr<Entity> > entities;

   std::vector< std::shared_ptr<Component> > components;

   std::vector< std::vector< std::shared_ptr<Shape> > > meshes;

   std::vector< std::shared_ptr<Uber> > ubers;

   std::shared_ptr<Skybox> skybox;

   std::shared_ptr<Terrain> initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain);

   void apply(std::shared_ptr<Application> app);

   void dump();

};

#endif
