#pragma once
#ifndef VOYAGER_HUD_H_INCLUDED
#define VOYAGER_HUD_H_INCLUDED

#include <voyager-utils/include/Texture.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <GLFW/glfw3.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include <string>

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

  // Sets the window to work in and establishes the frame.
  Hud(GLFWwindow* window, std::string resourcedir);
  // Creates new frame, start adding widgets
  void start();
  // Render all of the current widgets
  void render();
  // Loops through doc array and generates the widgests specified by the json file
  void generate();
  // create a textbox widget
  void textbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int width = 0, int heigth = 0, int r = 1, int g = 1, int b = 1, int a = 1);
  // No wdith or height defaults to have buton be same size as text.
  void button(const char *titlebar, const char *txt, int x_pos, int y_pos, int width = 0, int height = 0);
  // Draw image to screen
  void image(const char *filename, const char *titlebar, int x_pos, int y_pos, int i_width, int i_height);

private:
  // Loads resources/hud.json into doc
  void open();
};

#endif


/*
TODO:
 - FPS call
 - Image button
 - Text Button
 - Progress bar
 - Classes for each of the widget types, make hud a list of those classes
*/
