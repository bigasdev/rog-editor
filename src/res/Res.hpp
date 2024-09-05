#pragma once
#include "SDL_render.h"
#include "SDL_stdinc.h"
#ifndef RES_HPP
#define RES_HPP
#include "SDL_ttf.h"
#include "SDL_gpu.h"
#include "../tools/Common.hpp"

#include <iostream>
#include <map>
#include <vector>

class AssetData;

struct AsepriteHelper
{
  std::string file;
  std::string last_edited_time;
};


class Res {
public:
  Res(SDL_Renderer* renderer);
  ~Res();

  void init();
  void update();

  void load_folder(std::string path);

  // loaders
  void load_fonts();
  void load_sounds();
  void load_aseprites();
  void load_pallete();
  void load_shaders();

  //getters 
  TTF_Font *get_font(std::string name) { return m_fonts[name]; }
  Col get_color(int idx) { return m_palette[idx]; };
  PCol get_color_primitive(int idx) { return m_palette[idx].to_pcol(); };
  GPU_Image **get_texture(std::string name);
  std::vector<std::string> get_shaders() { return m_shaders; }
  Uint32 get_shader_id();
  GPU_ShaderBlock get_shader_block();

  void create_asset_data(std::string file, std::string name, std::string folder);

private:
  std::map<std::string, GPU_Image*> m_textures;
  std::map<std::string, AssetData*> m_assets;
  std::map<std::string, GPU_Image*> m_aseprite_textures;
  std::map<std::string, TTF_Font*> m_fonts;
  std::vector<std::string> m_shaders;
  std::vector<Uint32> m_shaders_id;
  std::vector<GPU_ShaderBlock> m_shader_blocks;
  std::vector<AsepriteHelper> m_aseprite_files;
  std::vector<Col> m_palette;

  SDL_Renderer *m_renderer;
};
#endif
