#pragma once
#ifndef _AIENGINE_H
#define _AIENGINE_H

#include <voyager-core/include/Engine.h>
#include "BrainComponent.h"

#include "NavMapEntity.h"

class AiEngine :
   public Engine
{
public:
   virtual void init();
   virtual void execute(double delta_time = 0) override;
   virtual void removeFlagged();
   void setNavMapEntity(std::shared_ptr<NavMapEntity> nav_map_entity) { this->nav_map_entity = nav_map_entity; }
private:
   std::shared_ptr<NavMapEntity> nav_map_entity = nullptr;

   void runBrain(double delta_time, std::shared_ptr<BrainComponent> brain);
};

#endif
