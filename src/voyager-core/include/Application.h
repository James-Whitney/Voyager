#pragma once
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <voyager-utils/include/GLSL.h>
#include <voyager-utils/include/Time.h>

#include "ApplicationType.h"
#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "WindowManager.h"

class Application : public EventCallbacks {

public:

   // constructors/destructors
   Application(ApplicationType, std::string);

   // getters/setters
   ApplicationType getType() { return this->type; }

   std::string getResourceDir() { return this->resource_dir; }
   void setResourceDir(std::string resource_dir) {
      this->resource_dir = resource_dir;
   }

   std::shared_ptr<WindowManager> getWindowManager() { return this->window; }
   void setWindowManager(std::shared_ptr<WindowManager> window) { this->window = window; }

   void setRenderEngine(std::shared_ptr<Engine> render_engine) { this->render_engine = render_engine; }

   // event callbacks
   virtual void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
   virtual void mouseCallback(GLFWwindow *window, int button, int action, int mods);
   virtual void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
   virtual void scrollCallback(GLFWwindow *window, double dX, double dY);
   virtual void resizeCallback(GLFWwindow *window, int in_width, int in_height);

   void run();

private:

   ApplicationType type;

   std::string resource_dir;
   std::shared_ptr<WindowManager> window;

   std::vector< std::shared_ptr<Entity> > things;

   LoopTimer timer = LoopTimer(10); // 10ms ma time step

   std::shared_ptr<Engine> render_engine;

   void init();                     // called once at the beginning
   void update(double delta_time);  // game update
   void render();                   // render the scene
   void shutdown();                 // called once at the end

   bool shouldQuit();

};

#endif
