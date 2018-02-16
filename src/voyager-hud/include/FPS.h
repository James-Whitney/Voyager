#pragma once
#ifndef VOYAGER_FPS_H_INCLUDED
#define VOYAGER_FPS_H_INCLUDED

#include "Widget.h"
#include <voyager-utils/include/Texture.h>
#include <imgui/imgui.h>

class FPS : public Widget {
public:
   FPS(const char *titlebar, int x_pos, int y_pos)
      : Widget(titlebar, x_pos, y_pos) { }

   void render() {
      Widget::setupDefaults();
      ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
      Widget::endDefault();
   }
};

#endif
