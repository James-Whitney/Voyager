#pragma once
#ifndef VOYAGER_TEXTBOX_H_INCLUDED
#define VOYAGER_TEXTBOX_H_INCLUDED

#include "Widget.h"
#include <imgui/imgui.h>

class Textbox : public Widget {
public:
  const char *text;
  ImGui::ImVec4 color;
};

#endif
