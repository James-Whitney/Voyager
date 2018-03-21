#pragma once
#ifndef VOYAGER_PROGRESSBAR_H_INCLUDED
#define VOYAGER_PROGRESSBAR_H_INCLUDED

#include "Widget.h"
#include <imgui/imgui.h>
#include <voyager-actors/include/HelmComponent.h>

#include <iostream>

class ProgressBar : public Widget {
public:
   float *percent;
   ImVec2 size;

   ProgressBar(const char *titlebar, float *start_percent, int x, int y, float width, float height)
      : Widget(titlebar, x, y)
   {
      this->percent = start_percent;
      this->size = ImVec2(width,height);
   }

   void render() {
      Widget::setupDefaults();
      ImGui::ProgressBar(*this->percent/1000, this->size);
      Widget::endDefault();
   }
};

#endif
