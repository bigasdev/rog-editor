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

std::string m_selected_pallete;

//settings 
int sprite_x = 8, sprite_y = 8;

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

  entities();
  atlas();
  pallete();

  ImGui::PopStyleColor();
  ImGui::End();
}

void AssetView::entities(){
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 430));
  ImGui::BeginChild("Entities", ImVec2(300, 150), true);
  ImGui::Text("-  Entities --------------");
  ImGui::BeginTabBar("Groups");
  ImGui::Button("", ImVec2(26, 20));
  ImGui::SameLine();
  ImGui::InputText("##Search", m_search_entity, IM_ARRAYSIZE(m_search_entity));
  ImGui::SameLine();
  if(ImGui::BeginTabItem("X")){
    ImGui::Text("X");
    ImGui::EndTabItem();
  }
  if(ImGui::BeginTabItem("Y")){
    ImGui::Text("Y");
    ImGui::EndTabItem();
  }
  ImGui::EndTabBar();
  ImGui::EndChild();
}

// Components
void AssetView::atlas(){
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 270));
  ImGui::BeginChild("Atlas", ImVec2(300, 150), true);
  ImGui::Text("-  Atlas --------------");
  if(m_selected_pallete != ""){
    auto asset = *g_res->get_texture(m_selected_pallete);
    auto x = asset->texture_w;
    auto y = asset->texture_h;
    auto row = 0;

    for(int i = 0; i < x; i+= sprite_x){
      for(int j = 0; j < y; j+= sprite_y){
        ImGui::ImageButton("t", (void*)(intptr_t)ImGuiHelper::convert_to_imgui(*g_res->get_texture(m_selected_pallete)), ImVec2(48,48), ImVec2((float)i/x, (float)j/y), ImVec2((float)(i+sprite_x)/x, (float)(j+sprite_y)/y));
        ImGui::SameLine();
        row++;

        if(row == 5){
          row = 0;
          ImGui::NewLine();
        }
      }
    }
  }
  ImGui::EndChild();
}


void AssetView::pallete() {
  ImGui::SetNextWindowPos(ImVec2(85, g_engine->get_window_size()->y - 110));
  ImGui::BeginChild(" Pallete", ImVec2(300, 100), true);
  ImGui::Text("- Pallete --------------");
  ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));
  for (auto &[key, value] : m_sprites) {
    if(m_selected_pallete == key){
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.1, 0.45, 1.0));
      ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0);
    }

    if (ImGui::Button((" " + key).c_str(), ImVec2(150, 18))) {
      m_selected_pallete = key;
      break;
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
      ImGui::BeginTooltip();
      ImGui::Image((void*)(intptr_t)ImGuiHelper::convert_to_imgui(*g_res->get_texture(key)), ImVec2(400,400));
      ImGui::EndTooltip();
    }

    if(m_selected_pallete == key){
      ImGui::PopStyleColor();
      ImGui::PopStyleVar();
    }
  }
  ImGui::PopStyleVar();
  ImGui::EndChild();
}
