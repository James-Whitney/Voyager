#pragma once
#ifndef VOYAGER_HUD_H_INCLUDED
#define VOYAGER_HUD_H_INCLUDED

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <GLFW/glfw3.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

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

  // Sets the window to work in and establishes the frame.
  Hud(GLFWwindow* window);
  // Creates new frame, start adding widgets
  void start();
  // Render all of the current widgets
  void render();
  // Loops through doc array and generates the widgests specified by the json file
  void generate();
  // create a textbox widget
  void textbox(const char *titlebar, const char * txt, int x_pos, int y_pos, int width, int heigth);
  // No wdith or height defaults to have buton be same size as text.
  void button(const char* txt, int width = 0, int height = 0);

  void image(std::string filename, int w_width, int w_height);

private:
  // Loads resources/hud.json into doc
  void open();
};

#endif
