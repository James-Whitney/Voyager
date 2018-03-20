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
#include <voyager-actors/include/HelmComponent.h>
#include <voyager-actors/include/TurretComponent.h>

#include <voyager-core/include/Application.h>

#include <iostream>

class Scene : public SceneMesh,
   public std::enable_shared_from_this<Scene>
{
public:

   std::vector< std::shared_ptr<Entity> > entities;

   std::vector< std::shared_ptr<Component> > components;

   std::shared_ptr<Skybox> skybox;

   void initTerrain(shared_ptr<Application> app, shared_ptr<Entity> terrain);

   void apply(std::shared_ptr<Application> app);

   void dump();

};

#endif
