#pragma once
#ifndef VOYAGER_WIDGET_H_INCLUDED
#define VOYAGER_WIDGET_H_INCLUDED

class Widget {
public:
  static const ImGuiWindowFlags beginDefaults = \
    ImGuiWindowFlags_NoTitleBar |
    ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoCollapse;

  int x_pos;
  int y_pos;
  const char *titlebar;
  int width;
  int height;
};

#endif
