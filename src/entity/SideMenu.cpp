#include "SideMenu.hpp"
#include <iostream>
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"

SideMenu::SideMenu(){
}

void SideMenu::show(){
  ImGui::Begin("Side Menu");
  ImGui::Button("");
  ImGui::End();
}

