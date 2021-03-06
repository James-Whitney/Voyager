#pragma once
#ifndef _AIENGINE_H
#define _AIENGINE_H

#include <voyager-core/include/Engine.h>
#include "BrainComponent.h"

#include "../enemies/Spawner.h"
#include "NavMapEntity.h"

class AiEngine :
   public Engine
{
public:
   virtual void init() override;
   virtual void execute(double delta_time = 0) override;
   virtual void removeFlagged() override;
   virtual void registerComponent(std::shared_ptr<Component> c) override;
   void setNavMapEntity(std::shared_ptr<NavMapEntity> nav_map_entity) { this->nav_map_entity = nav_map_entity; }
private:
   std::shared_ptr<NavMapEntity> nav_map_entity = nullptr;
   std::vector<std::shared_ptr<Spawner>> spawners;

   void runBrain(double delta_time, std::shared_ptr<BrainComponent> brain);
};

#endif
