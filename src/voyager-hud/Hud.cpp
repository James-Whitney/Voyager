#include "include/Hud.h"
#include <cstdio>

#include <iostream>

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>

Hud::Hud(GLFWwindow* window, std::string resourcedir) {
   resource_dir = resourcedir;
   ImGui_ImplGlfwGL3_Init(window, true);
   glfwGetWindowSize(window, &width, &height);
   Hud::open();
}
// Creates new frame
void Hud::start() {
   ImGui_ImplGlfwGL3_NewFrame();
}
void Hud::render() {
   ImGui::SetNextWindowPos(ImVec2(0.01*Hud::width, 0.98*Hud::height), 0, ImVec2(0.5f,0.5f));
   ImGui::SetNextWindowSize(ImVec2(0,0), 0);
   ImGui::SetNextWindowBgAlpha(1.0f);
   ImGui::Begin("fps", NULL, Hud::beginDefaults);
   ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
   ImGui::End();

   ImGui::SetNextWindowPos(ImVec2(0.5*Hud::width, 0.5*Hud::height), 0, ImVec2(0.5f,0.5f));
   ImGui::SetNextWindowSize(ImVec2(0,0), 0);
   ImGui::SetNextWindowBgAlpha(1.0f);
   ImGui::Begin("progress", NULL, Hud::beginDefaults);
   ImGui::ProgressBar(0.75, ImVec2(0.11*Hud::width,0.66*Hud::height));
   ImGui::End();

   for (auto& widget : this->widgets) {
      widget->render();
   }
   ImGui::Render();
}

void Hud::open() {
   FILE* fp = fopen("resources/hud.json", "r"); // non-Windows use "r"
   char readBuffer[65536];
   rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
   if (doc.ParseStream(is).HasParseError()) {
      fprintf(stderr, "\nError(offset %u): %s\n",
         (unsigned)doc.GetErrorOffset(),
         GetParseError_En(doc.GetParseError()));
   }
   fclose(fp);

   Hud::generate();
}


void Hud::generate() {
   for (auto& widget : doc.GetArray()) {
      if (!strcmp("textbox", widget["type"].GetString())) {
         this->widgets.push_back(new Textbox(widget["titlebar"].GetString(), widget["text"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["r"].GetInt(), widget["g"].GetInt(), widget["b"].GetInt(), widget["a"].GetInt()));
      } //else if (!strcmp("button", widget["type"].GetString())) {
         /*Hud::button(widget["titlebar"].GetString(), widget["text"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height);
      } */
      else if (!strcmp("image", widget["type"].GetString())) {
         this->widgets.push_back(new Image(widget["image"].GetString(), widget["titlebar"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["width"].GetInt(), widget["height"].GetInt()));
      }
   }
}
