#include "SideMenu.hpp"
#include <cstdint>
#include <iostream>
#include "../imgui/imgui_impl_opengl3.h"
#include "SDL.h"
#include "../core/global.hpp"
#include "../core/Engine.hpp"
#include "SDL_gpu.h"
#include "../res/Res.hpp"

GPU_Image* sdl_img;
GPU_TextureHandle t;

//NOTE: Remember this is how we can load images to the dear imgui, we load from gpu and then get the texture handle
SideMenu::SideMenu(){
  sdl_img = GPU_LoadImage("res/sprites/icons/global.png");
  GPU_SetImageFilter(sdl_img, GPU_FILTER_NEAREST);
  t = GPU_GetTextureHandle(sdl_img);
  std::cout << t << std::endl;
  if(sdl_img == nullptr){
    std::cout << "Error loading image" << std::endl;
  }else{
    std::cout << "Image loaded" << std::endl;
  }
}

void SideMenu::show(){
  ImGui::SetNextWindowPos(ImVec2(0, 18.5f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1, 0.1, 0.1, 1.0));
  ImGui::BeginChild("SideMenu", ImVec2(69, g_engine->get_window_size()->y), true);
  
  if(ImGui::ImageButton("assets", (void*)(intptr_t)t, ImVec2(48, 48))){

    m_state = State::ASSET;
  }
  if(ImGui::ImageButton("none", (void*)(intptr_t)t, ImVec2(48, 48))){
    m_state = State::NONE;
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
  ImGui::EndChild();
}

