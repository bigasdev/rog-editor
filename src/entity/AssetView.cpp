#include "AssetView.hpp"
#include "../core/Engine.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui_impl_opengl3.h"
#include "../renderer/Sprite.hpp"
#include "../res/Res.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include <cstdint>
#include <iostream>
#include "../tools/ImGuiHelper.hpp"
#include <memory>

#include "InfoBar.hpp"

std::unique_ptr<InfoBar> info_bar;
std::map<std::string, Sprite> m_sprites;

AssetView::AssetView(std::map<std::string, Sprite> sprites) {
  info_bar = std::make_unique<InfoBar>();

  m_sprites = sprites;
}

void AssetView::show() {
  ImGui::SetNextWindowPos(ImVec2(75, 20.0f));
  ImGui::SetNextWindowSize(ImVec2(g_engine->get_window_size()->x - 80,
                                  g_engine->get_window_size()->y - 25));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05, 0.05, 0.05, 1.0));
  ImGui::Begin(" Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  info_bar->show();
  pallete();

  ImGui::PopStyleColor();
  ImGui::End();
}

// Components
void AssetView::pallete() {
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 110));
  ImGui::BeginChild(" Pallete", ImVec2(200, 100), true);
  ImGui::Text("- Pallete --------------");
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));
  for (auto &[key, value] : m_sprites) {
    if (ImGui::Button((" " + key).c_str(), ImVec2(150, 18))) {
    }
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
      ImGui::BeginTooltip();
      ImGui::Image((void*)(intptr_t)ImGuiHelper::convert_to_imgui(*g_res->get_texture(key)), ImVec2(200,200));
      ImGui::EndTooltip();
    }
  }
  ImGui::PopStyleVar();
  ImGui::EndChild();
}
