#pragma once
#ifndef _NAVMAPENTITY_H
#define _NAVMAPENTITY_H

#include <memory>

#include <voyager-core/include/Entity.h>
#include <voyager-render/include/Renderable.h>

#include "NavMap.h"

#define NAV_MAP_DO_RENDER 1 // set to 1 to render the nav map

class NavMapRenderable;

class NavMapEntity :
   public Entity
{
public:
   NavMapEntity(ent_ptr_t player, std::shared_ptr<Terrain> terrain_shape);
   std::shared_ptr<NavMap> getNavMap() { return this->nav_map; }
private:
   std::shared_ptr<NavMap> nav_map;
};

class NavMapRenderable : public Renderable {
public:
   NavMapRenderable(std::shared_ptr<NavMap> nav_map);
   virtual void init() override;
   virtual void renderableInit(std::string resource_dir) override;
   virtual bool hasCustomProgram() override { return true; }
   virtual void draw(std::shared_ptr<Program> prog) override;
private:
   std::shared_ptr<NavMap> nav_map;
   std::shared_ptr<Program> prog;
};

#endif
