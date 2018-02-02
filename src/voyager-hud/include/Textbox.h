#pragma once
#ifndef VOYAGER_TEXTBOX_H_INCLUDED
#define VOYAGER_TEXTBOX_H_INCLUDED

#include "Widget.h"
#include <imgui/imgui.h>

class Textbox : public Widget {
public:
  const char *text;
  ImVec4 color;

  Textbox(const char *titlebar, const char *txt, int x_pos, int y_pos, int r, int g, int b, int a)
    : Widget(titlebar, x_pos, y_pos)
  {
    this->text = txt;
    this->color = ImVec4(r,g,b,a);
  }

  void render() {
    Widget::setupDefaults();
    ImGui::TextColored(this->color, this->text);
    Widget::endDefault();
  }
};

#endif
