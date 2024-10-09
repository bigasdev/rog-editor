#include "AssetView.hpp"
#include "../core/Engine.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui_impl_opengl3.h"
#include "../res/Res.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include <cstdint>
#include <iostream>

AssetView::AssetView() {}

void AssetView::show() {
  ImGui::Begin("Assets", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

  ImGui::End();
}
