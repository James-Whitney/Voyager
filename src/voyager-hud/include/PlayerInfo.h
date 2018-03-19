#pragma once
#ifndef VOYAGER_PLAYERINFO_H_INCLUDED
#define VOYAGER_PLAYERINFO_H_INCLUDED

#include "Textbox.h"
#include <imgui/imgui.h>

#include <voyager-actors/include/PlayerComponent.h>

#include <iostream>

class PlayerInfo : public Textbox {
public:
   float direction;
   const char *dir[4] = {"Fore", "Port", "Aft", "Starboard"};
   ImVec2 size;

   std::shared_ptr<PlayerComponent> player;

   PlayerInfo(const char *titlebar, int x_pos, int y_pos,
      int r, int g, int b, int a, std::shared_ptr<PlayerComponent> player)
      : Textbox(titlebar, "", x_pos, y_pos, r, g, b, a) {
      this->player = player;
   }

   void render() {
      printf("%f\n", player->getRotation());
      //this->text = std::to_string(player->getRotation()).c_str();
      Textbox::render();
   }
};

#endif
