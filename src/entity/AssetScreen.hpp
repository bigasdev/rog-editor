#pragma once 

#include "../tools/Common.hpp"

class AssetScreen{
public:
  AssetScreen();
  ~AssetScreen();

  void update();
  void root();
  void ent();
  void ui();
private:
  int m_zoom = 8;

  vec2 m_grid_size = {16,16};
};
