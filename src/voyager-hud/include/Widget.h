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

   Widget(const char *title, int x, int y) {
      this->titlebar = title;
      this->x_pos = x;
      this->y_pos = y;
   }

   virtual void render() { };

   void setupDefaults() {
      ImGui::SetNextWindowPos(ImVec2(this->x_pos, this->y_pos), 0, ImVec2(0.5f,0.5f));
      ImGui::SetNextWindowSize(ImVec2(0,0), 0);
      ImGui::SetNextWindowBgAlpha(0.0f);
      ImGui::Begin(this->titlebar, NULL, this->beginDefaults);
   }

   void endDefault() {
      ImGui::End();
   }

};

#endif
