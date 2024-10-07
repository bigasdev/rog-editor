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
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("Side Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
  ImGui::Button("");
  
  ImGui::ImageButton("hello", (void*)(intptr_t)t, ImVec2(48, 48));
  ImGui::ImageButton("hello", (void*)(intptr_t)t, ImVec2(16, 16));
  ImGui::End();
}

