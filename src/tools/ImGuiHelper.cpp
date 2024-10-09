#include "ImGuiHelper.hpp"
#include "SDL_gpu.h"

GPU_TextureHandle ImGuiHelper::convert_to_imgui(GPU_Image* img){
  return GPU_GetTextureHandle(img);
}
