#include "InfoBar.hpp"
#include "../imgui/imgui.h"
#include "../tools/Mouse.hpp"

InfoBar::InfoBar() {}

void InfoBar::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 25));
  ImGui::SetNextWindowSize(ImVec2(190, 15));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20);
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 1.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.62, 0.36, 0.95, 1.0));
  ImGui::Begin("Info Bar", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
  ImGui::BeginGroup();
  ImGui::Text("Cursor Position: %d, %d",
              static_cast<int>(Mouse::get_mouse_pos().x),
              static_cast<int>(Mouse::get_mouse_pos().y));
  ImGui::EndGroup();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
  ImGui::PopStyleColor();
  ImGui::End();
}
