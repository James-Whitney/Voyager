#pragma once
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ApplicationType.h"
#include "Entity.h"

class Application {

public:

   // constructors/destructors
   Application(ApplicationType, std::string);

   // getters/setters
   ApplicationType getType() { return this->type; }

   void run();

private:

   ApplicationType type;

   std::string resource_dir;
   std::vector< std::shared_ptr<Entity> > things;

   void init();            // called once at the beginning
   void update();          // game update
   void render();          // render the scene
   void shutdown();        // called once at the end

};

#endif
