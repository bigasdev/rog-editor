#pragma once
#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "Common.hpp"
namespace Mouse
{

  bool is_at_area(Rect pArea, int width, int height);
  Rect get_mouse_area(int width, int height);
  vec2 get_mouse_pos();
  float get_mouse_angle(vec2 pos);
}; // namespace Mouse

#endif
