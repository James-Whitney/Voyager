#pragma once
#ifndef VOYAGER_SHIPINFO_H_INCLUDED
#define VOYAGER_SHIPINFO_H_INCLUDED

#include "Widget.h"
#include <imgui/imgui.h>

#include <voyager-actors/include/HelmComponent.h>

#include <iostream>

class ShipInfo : public Widget {
public:
   float min;
   float max;
   ImVec2 size;

   std::shared_ptr<HelmComponent>  ship;

   ShipInfo(const char *titlebar, int x, int y, float width, float height,
      float minFloat, float maxFloat, std::shared_ptr<HelmComponent> ship)
      : Widget(titlebar, x, y) {
      this->min = minFloat;
      this->max = maxFloat;
      this->ship = ship;
      this->size = ImVec2(width,height);
   }

   void render() {
      Widget::setupDefaults();
      ImGui::VSliderFloat(this->titlebar, this->size, this->ship->getForwardThrottle(), this->min, this->max, "");
      Widget::endDefault();
   }
};

#endif
