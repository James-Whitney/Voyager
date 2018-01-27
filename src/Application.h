#pragma once
#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "utils/GLSL.h"
#include "utils/WindowManager.h"
#include "entities/Entity.h"

class Application : public EventCallbacks {

public:

   // constructors/destructors
   Application(std::string);

   // getters/setters
   std::shared_ptr<WindowManager> getWindowManager() { return this->window; }
   void setWindowManager(std::shared_ptr<WindowManager> window) { this->window = window; }

   // interaction callbacks
   void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
   void mouseCallback(GLFWwindow *window, int button, int action, int mods);
   void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
   void scrollCallback(GLFWwindow *window, double dX, double dY);
   void resizeCallback(GLFWwindow *window, int in_width, int in_height);

   void run();

private:

   std::string resource_dir;
   std::shared_ptr<WindowManager> window;

   std::vector<std::shared_ptr<Entity>> things;

   void init();            // called once at the beginning
   void update();          // game update
   void render();          // render the scene
   void shutdown();        // called once at the end

};

#endif
