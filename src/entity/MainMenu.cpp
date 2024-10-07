#include "MainMenu.hpp"
#include "../imgui/imgui.h"

MainMenu::MainMenu() {}

void MainMenu::show() {
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("New")) {
      // New();
    }
    if (ImGui::MenuItem("Open", "Ctrl+O")) {
      // Open();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {
      // Save();
    }
    if (ImGui::MenuItem("Save As..")) {
      // SaveAs();
    }
    ImGui::Separator();
    if (ImGui::MenuItem("Exit")) {
      // Exit();
    }
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Edit")) {
    if (ImGui::MenuItem("Undo", "CTRL+Z")) {
      // Undo();
    }
    if (ImGui::MenuItem("Redo", "CTRL+Y")) {
      // Redo();
    }
    ImGui::EndMenu();
  }

  ImGui::EndMainMenuBar();
}
