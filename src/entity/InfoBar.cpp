#include "InfoBar.hpp"
#include "../imgui/imgui.h"
#include "../tools/Mouse.hpp"

InfoBar::InfoBar() {}

void InfoBar::show() {
  ImGui::SetNextWindowPos(ImVec2(85, 45));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1, 0.1, 0.1, 1.0));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.62, 0.36, 0.95, 1.0));
  ImGui::BeginChild("Info Bar", ImVec2(190, 30), true);
  ImGui::BeginGroup();
  ImGui::Text("Cursor Position: %d, %d",
              static_cast<int>(Mouse::get_mouse_pos().x),
              static_cast<int>(Mouse::get_mouse_pos().y));
  ImGui::EndGroup();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
  ImGui::PopStyleColor();
  ImGui::EndChild();
}
