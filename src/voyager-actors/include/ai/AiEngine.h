#pragma once
#ifndef _AIENGINE_H
#define _AIENGINE_H

#include <voyager-core/include/Engine.h>

#include "NavMapEntity.h"

class AiEngine :
   public Engine
{
public:
   virtual void init();
   virtual void execute(double delta_time = 0);
   void setNavMapEntity(std::shared_ptr<NavMapEntity> nav_map_entity) { this->nav_map_entity = nav_map_entity; }
private:
   std::shared_ptr<NavMapEntity> nav_map_entity = nullptr;
};

#endif