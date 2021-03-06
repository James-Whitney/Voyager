#include "include/Hud.h"
#include <cstdio>

#include <iostream>

#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>

Hud::Hud(GLFWwindow* window, std::string resourcedir,
   std::shared_ptr<HelmComponent>  ship, std::shared_ptr<PlayerComponent>  player)
{
   resource_dir = resourcedir;
   ImGui_ImplGlfwGL3_Init(window, false);
   glfwGetWindowSize(window, &width, &height);
   this->ship = ship;
   this->player = player;
   Hud::open();
   startScreen = false;
   glfwSetCharCallback(window, ImGui_ImplGlfwGL3_CharCallback);
   this->buf[0] = 0;
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

void Hud::startMenu() {
   ImGui::SetNextWindowPos(ImVec2(0.5*Hud::width, 0.5*Hud::height), 0, ImVec2(0.5f,0.5f));
   ImGui::SetNextWindowSize(ImVec2(0,0), 0);
   ImGui::SetNextWindowBgAlpha(0.0f);
   ImGui::Begin("Server Ip Input", NULL, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse);
   ImGui::InputText("serverIp", this->buf, 1024);
   if (ImGui::GetIO().KeysDown[GLFW_KEY_ENTER]) {
      this->startScreen = false;
   }
   ImGui::End();
   ImGui::Render();
}

void Hud::dynamicTextbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int r, int g, int b, int a) {
   Textbox *textbox = new Textbox(titlebar, txt, x_pos, y_pos, r, g, b, a);
   textbox->render();
}


void Hud::generate() {
   for (auto& widget : doc.GetArray()) {
      if (!strcmp("textbox", widget["type"].GetString())) {
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
         this->widgets.push_back(new ProgressBar(widget["titlebar"].GetString(),
            this->ship->getShip()->getEntity()->getHealth(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["width"].GetFloat()*Hud::width, widget["height"].GetFloat()*Hud::height));
      } else if (!strcmp("shipinfo", widget["type"].GetString())) {
         this->widgets.push_back(new ShipInfo(widget["titlebar"].GetString(),
            widget["x_pos"].GetFloat(), widget["y_pos"].GetFloat(),
            widget["width"].GetFloat(), widget["height"].GetFloat(),
            this->ship->getMinForwardThrottle(), this->ship->getMaxForwardThrottle(), this->ship));
      } else if (!strcmp("playerinfo", widget["type"].GetString())) {
         this->widgets.push_back(new PlayerInfo(widget["titlebar"].GetString(),
            widget["x_pos"].GetFloat()*Hud::width, widget["y_pos"].GetFloat()*Hud::height,
            widget["r"].GetInt(), widget["g"].GetInt(), widget["b"].GetInt(), widget["a"].GetInt(),
            this->player));
      }

   }
}

void Hud::guiKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
   ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
}
