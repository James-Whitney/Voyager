#pragma once
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <voyager-utils/include/GLSL.h>
#include <voyager-utils/include/Time.h>
//#include <voyager-utils/include/IdMap.h>
#include <voyager-render/include/RenderEngine.h>

#include "ApplicationType.h"
#include "Component.h"
#include "Engine.h"
#include "Entity.h"
#include "WindowManager.h"
#include "VoyagerConfig.h"

class Application : public EventCallbacks {

public:

   // constructors/destructors
   Application();

   // getters/setters
   std::shared_ptr<VoyagerConfig> getConfig() { return this->config; }
   void setConfig(std::shared_ptr<VoyagerConfig> config) { this->config = config; }

   ApplicationType getType() { return this->config->type; }
   std::string getResourceDir() { return this->config->resource_dir; }

   std::shared_ptr<WindowManager> getWindowManager() { return this->window; }
   void setWindowManager(std::shared_ptr<WindowManager> window) { this->window = window; }

   std::shared_ptr<Engine> getRenderEngine() { return this->render_engine; }
   void setRenderEngine(std::shared_ptr<Engine> render_engine) { this->render_engine = render_engine; }

   std::shared_ptr<Engine> getPhysicsEngine() { return this->physics_engine; }
   void setPhysicsEngine(std::shared_ptr<Engine> physics_engine) { this->physics_engine = physics_engine; }

   std::shared_ptr<Engine> getActorEngine() { return this->actor_engine; }
   void setActorEngine(std::shared_ptr<Engine> actor_engine) { this->actor_engine = actor_engine; }

   std::unordered_map<long, std::shared_ptr<Entity> > getThings() { return this->things; }

   // event callbacks
   virtual void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
   virtual void mouseCallback(GLFWwindow *window, int button, int action, int mods);
   virtual void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
   virtual void scrollCallback(GLFWwindow *window, double dX, double dY);
   virtual void resizeCallback(GLFWwindow *window, int in_width, int in_height);

   void run();

private:

   std::shared_ptr<VoyagerConfig> config;

   std::shared_ptr<WindowManager> window;

   std::unordered_map< long, std::shared_ptr<Entity> > things;

   LoopTimer timer = LoopTimer(10); // 10ms max time step

   std::shared_ptr<Engine> render_engine;
   std::shared_ptr<Engine> physics_engine;
   std::shared_ptr<Engine> actor_engine;

   void init();                     // called once at the beginning
   void update(double delta_time);  // game update
   void render();                   // render the scene
   void physics(double delta_time); // run the physics engine
   void actors(double delta_time);  // run the actors engine
   void shutdown();                 // called once at the end

   bool shouldQuit();

};

#endif
