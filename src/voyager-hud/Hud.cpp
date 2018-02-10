#include "include/Hud.h"
#include <cstdio>

#include <iostream>

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>

Hud::Hud(GLFWwindow* window, std::string resourcedir) {
   resource_dir = resourcedir;
   ImGui_ImplGlfwGL3_Init(window, false);
   glfwGetWindowSize(window, &width, &height);
   Hud::open();
}
// Creates new frame
void Hud::start() {
   ImGui_ImplGlfwGL3_NewFrame();
}

void Hud::render() {
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
         if (!strcmp("compass-text", widget["titlebar"].GetString())) { this->compassIdx = this->widgets.size(); }
         this->widgets.push_back(new Textbox(widget["titlebar"].GetString(), widget["text"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["r"].GetInt(), widget["g"].GetInt(), widget["b"].GetInt(), widget["a"].GetInt()));
      } else if (!strcmp("fps", widget["type"].GetString())) {
         this->widgets.push_back(new FPS(widget["titlebar"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height));
      } else if (!strcmp("image", widget["type"].GetString())) {
         this->widgets.push_back(new Image(widget["image"].GetString(), widget["titlebar"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["width"].GetInt(), widget["height"].GetInt()));
      } else if (!strcmp("progress", widget["type"].GetString())) {
         if (!strcmp("health-bar", widget["titlebar"].GetString())) { this->healthIdx = this->widgets.size(); }
         this->widgets.push_back(new ProgressBar(widget["titlebar"].GetString(), widget["percent"].GetFloat(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height));
      }
   }
}
