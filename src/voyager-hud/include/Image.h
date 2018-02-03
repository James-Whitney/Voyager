#pragma once
#ifndef VOYAGER_IMAGE_H_INCLUDED
#define VOYAGER_IMAGE_H_INCLUDED

#include "Widget.h"
#include <voyager-utils/include/Texture.h>
#include <imgui/imgui.h>

class Image : public Widget {
public:
   Texture tex;
   ImVec2 img_size;

   Image(const char *filename, const char *titlebar, int x_pos, int y_pos, float i_width, float i_height)
      : Widget(titlebar, x_pos, y_pos)
   {
      this->img_size = ImVec2(i_width,i_height);

      tex = Texture();
      tex.setFilename(filename);
      tex.init();
   }

   void render() {
      Widget::setupDefaults();
      ImGui::Image((void *)tex.getID(), img_size);
      Widget::endDefault();
   }
};

#endif
