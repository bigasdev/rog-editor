#pragma once 
#include "SDL.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "SDL_gpu.h"
#include <cstdint>

namespace ImGuiHelper{
   GPU_TextureHandle convert_to_imgui(GPU_Image* img);
};
