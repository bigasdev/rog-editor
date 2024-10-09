#include "SideMenu.hpp"
#include "../core/Engine.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui_impl_opengl3.h"
#include "../res/Res.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include <cstdint>
#include <iostream>

GPU_Image *sdl_img;
GPU_TextureHandle t;

// NOTE: Remember this is how we can load images to the dear imgui, we load from
// gpu and then get the texture handle
SideMenu::SideMenu() {
  sdl_img = GPU_LoadImage("res/sprites/icons/global.png");
  GPU_SetImageFilter(sdl_img, GPU_FILTER_NEAREST);
  t = GPU_GetTextureHandle(sdl_img);
  if (sdl_img == nullptr) {
    std::cout << "Error loading image" << std::endl;
  } else {
    std::cout << "Image loaded" << std::endl;
  }
}

void SideMenu::show() {
  ImGui::SetNextWindowPos(ImVec2(0, 18.5f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1, 0.1, 0.1, 1.0));
  ImGui::BeginChild("SideMenu", ImVec2(69, g_engine->get_window_size()->y),
                    true);

  //NOTE: DONT REMOVE THIS 
  // ILL USE THIS LATER TO CREATE THE SPRITE ATLAS
  int sprite_x = 0; // X position of the sprite in the atlas
  int sprite_y = 16; // Y position of the sprite in the atlas
  int sprite_width = 16;
  int sprite_height = 16;
  int atlas_size = 500;


  // Top-left texture coordinates
  ImVec2 uv0 =
      ImVec2((float)sprite_x / 400, (float)sprite_y / 250);

  // Bottom-right texture coordinates
  ImVec2 uv1 = ImVec2((float)(sprite_x + sprite_width) / 400,
                      (float)(sprite_y + sprite_height) / 250);


  if (ImGui::ImageButton("assets", (void *)(intptr_t)t, ImVec2(48, 48))) {

    m_state = State::ASSET;
  }
  if (ImGui::ImageButton("none", (void *)(intptr_t)t, ImVec2(48, 48))) {
    m_state = State::NONE;
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
  ImGui::EndChild();
}
