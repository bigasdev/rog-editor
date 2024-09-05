#pragma once 
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../tools/Common.hpp"
#include "SDL_ttf.h"
#include <vector>
#include <iostream>

class GPU_Target;
class GPU_Image;
class GPU_ShaderProgram;
class GPU_ShaderBlock;
class Sprite;

class Renderer{
public: 
  Renderer(GPU_Target* gpu);
  ~Renderer();

  void post_update();
  void init_shader(std::vector<std::string> shaders);

  int get_calls() { return m_calls; }

  void draw_rect(Rect rect, Col color, bool fill = false);
  void draw_line(Line line, Col color);
  void draw_text(vec2 pos, const char* text, TTF_Font *font, Col color = {255,255,255,255}, int size = 1, int width = 128);
  void draw_from_sheet(GPU_Image *sheet, vec2 pos, Rect l_point, bool use_shader = false);
  void draw(GPU_Image *sheet, Sprite spr, vec2 pos);
private:
  GPU_Target* m_gpu;
  Uint32 m_shader;

  int m_calls = 0;
};

#endif
