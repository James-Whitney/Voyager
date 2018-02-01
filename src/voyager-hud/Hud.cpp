#include "include/Hud.h"
#include <cstdio>
#include <string>

#include <iostream>

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>

Hud::Hud(GLFWwindow* window) {
  ImGui_ImplGlfwGL3_Init(window, true);
  glfwGetWindowSize(window, &width, &height);
  Hud::open();
}
// Creates new frame
void Hud::start() {
  ImGui_ImplGlfwGL3_NewFrame();
}
void Hud::render() {
  ImGui::Render();
}
// create a textbox
void Hud::textbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int width, int height, int r, int g, int b, int a) {
  ImGui::SetNextWindowPos(ImVec2(x_pos, y_pos), 0, ImVec2(0.5f,0.5f));
  ImGui::SetNextWindowSize(ImVec2(0,0), 0);
  ImGui::SetNextWindowBgAlpha(0.0f);

  ImGui::Begin(titlebar, NULL, Hud::beginDefaults);
  ImGui::TextColored(ImVec4(0,0,0,1), txt);
  ImGui::End();
}

void Hud::button(const char *titlebar, const char *txt, int x_pos, int y_pos, int width, int height) {
  ImGui::Begin("", NULL, Hud::beginDefaults);
  ImGui::Button(txt, ImVec2(width, height));
  ImGui::End();
}


void Hud::image(const char *filename, const char *titlebar, int x_pos, int y_pos, int i_width, int i_height) {
  Texture tex = Texture();
  tex.setFilename(filename);
  tex.init();

  ImGui::SetNextWindowPos(ImVec2(x_pos, y_pos), 0, ImVec2(0.5f,0.5f));
  ImGui::SetNextWindowBgAlpha(0.0f);
  ImGui::SetNextWindowSize(ImVec2(0,0), 0);

  ImGui::Begin(titlebar, NULL, Hud::beginDefaults);
  ImGui::Image((void *)tex.getID(), ImVec2(i_width,i_height));
  ImGui::End();
}

void Hud::open() {
  FILE* fp = fopen("../resources/hud.json", "r"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  if (doc.ParseStream(is).HasParseError()) {
    fprintf(stderr, "\nError(offset %u): %s\n",
      (unsigned)doc.GetErrorOffset(),
      GetParseError_En(doc.GetParseError()));
  }
  fclose(fp);
}


void Hud::generate() {
  ImGui::SetNextWindowPos(ImVec2(0.01*Hud::width, 0.98*Hud::height), 0, ImVec2(0.5f,0.5f));
  ImGui::SetNextWindowSize(ImVec2(0,0), 0);
  ImGui::SetNextWindowBgAlpha(1.0f);
  ImGui::Begin("fps", NULL, Hud::beginDefaults);
  ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
  ImGui::End();

  for (auto& widget : doc.GetArray()) {
    if (!strcmp("textbox", widget["type"].GetString())) {
      Hud::textbox(widget["titlebar"].GetString(), widget["text"].GetString(),
        widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
        widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height);
    } else if (!strcmp("button", widget["type"].GetString())) {
      Hud::button(widget["titlebar"].GetString(), widget["text"].GetString(),
        widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
        widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height);
    } else if (!strcmp("image", widget["type"].GetString())) {
      Hud::image(widget["image"].GetString(), widget["titlebar"].GetString(),
        widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
        widget["width"].GetInt(), widget["height"].GetInt());
    }
  }
}
