#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

class Hud {
public:
  // Defaults for Begin()
  ImGuiWindowFlags beginDefaults = ImGuiWindowFlags_NoTitleBar |
                      ImGuiWindowFlags_NoResize |
                      ImGuiWindowFlags_NoMove |
                      ImGuiWindowFlags_NoCollapse;

  // Sets the window and establishes the frame.
  Hud(GLFWwindow* window) {
    ImGui_ImplGlfwGL3_Init(window, true);
  }
  // Creates new frame
  void start() { ImGui_ImplGlfwGL3_NewFrame(); }
  void render() { ImGui::Render(); }
  // create a textbox
  void textbox(const char * txt, int x_pos, int y_pos, int height, int width) {
    ImGui::SetNextWindowPos(ImVec2(x_pos, y_pos), 0, ImVec2(0.5f,0.5f));
    ImGui::SetNextWindowSize(ImVec2(height, width), 0);

    ImGui::Begin("", NULL, beginDefaults);
    ImGui::Text(txt);
    ImGui::End();
  }
};
