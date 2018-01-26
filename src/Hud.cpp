#include "Hud.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/error/en.h>
#include <cstdio>

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
void Hud::textbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int width, int height) {
  ImGui::SetNextWindowPos(ImVec2(x_pos, y_pos), 0, ImVec2(0.5f,0.5f));
  ImGui::SetNextWindowSize(ImVec2(width, height), 0);

  ImGui::Begin(titlebar, NULL, Hud::beginDefaults);
  ImGui::Text(txt);
  ImGui::End();
}

void Hud::button(const char* txt, int width, int height) {
  ImGui::Begin("", NULL, Hud::beginDefaults);
  ImGui::Button(txt, ImVec2(width, height));
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
  for (auto& widget : doc.GetArray()) {
    Hud::textbox(widget["titlebar"].GetString(), widget["text"].GetString(),
      widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
      widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height);
  }
}
