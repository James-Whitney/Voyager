
#pragma once
#ifndef VOYAGER_HUD_H_INCLUDED
#define VOYAGER_HUD_H_INCLUDED

#include <voyager-utils/include/Texture.h>
#include <voyager-core/include/WindowManager.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <GLFW/glfw3.h>

#include <rapidjson/include/rapidjson/rapidjson.h>
#include <rapidjson/include/rapidjson/document.h>

#include <voyager-actors/include/HelmComponent.h>

#include <string>
#include <vector>


#include "Widget.h"
#include "Image.h"
#include "Textbox.h"
#include "FPS.h"
#include "ProgressBar.h"

class Hud {
public:
   // Defaults for Begin()
   static const ImGuiWindowFlags beginDefaults = \
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoCollapse;

   int width;
   int height;

   GLuint tex_2d;
   rapidjson::Document doc;
   std::string resource_dir;
   std::vector<Widget*> widgets;

   int healthIdx;
   int compassIdx;

   bool startScreen;

   char buf[1024];

   // Sets the window to work in and establishes the frame.
   Hud(GLFWwindow* window, std::string resourcedir);

   void run() { start(); render(); }
   // Creates new frame, start adding widgets
   void start();
   // Render all of the current widgets
   void render();
   // Loops through doc array and generates the widgests specified by the json file
   void generate();

   void dynamicTextbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int r = 0, int g = 0, int b = 0, int a = 1);

   void startMenu();

   void shipStats(std::shared_ptr<HelmComponent>  ship);

   void guiKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
   bool inputScreen() { return this->startScreen; }

private:
   // Loads resources/hud.json into doc
   void open();

   std::shared_ptr<GLFWwindow> window;

};

#endif
