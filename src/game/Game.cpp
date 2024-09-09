#include "Game.hpp"
#include "../core/Engine.hpp"
#include "../core/InputManager.hpp"
#include "../core/Timer.hpp"
#include "../core/global.hpp"
#include "../imgui/imgui.h"
#include "../renderer/AppGui.hpp"
#include "../renderer/Camera.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/Sprite.hpp"
#include "../res/Res.hpp"
#include "../tools/Cooldown.hpp"
#include "../tools/Logger.hpp"
#include "../tools/Math.hpp"
#include "../tools/Mouse.hpp"
#include "DataLoader.hpp"
#include "SDL.h"
#include "SDL_gpu.h"

struct Asset{
  std::string file_name;
  Sprite spr;
};

std::string project_folder;
Sprite test;
vec2 pos = {0, 0};
std::map<std::string, Sprite> sprite_map;
std::vector<Asset> m_assets;
Asset m_selected_asset;
std::string selected_asset;

vec2 mouse_pos;
bool mouse_clicked;
bool mouse_wheel_clicked;

vec2 grid_ratio = {16, 16};
vec2 invisible_ratio = {16, 16};

Game::Game() {}

Game::~Game() {}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  /*project_folder = Data_Loader::load_folder("Select project folder");
  g_res->reset_aseprites();
  g_res->load_aseprites(project_folder + "/res/");*/

  auto files = g_res->get_aseprite_names();
  int x = 0;
  for (auto file : files) {
    Logger::log(file);

    auto spr = Sprite();
    spr.spr_x = x;
    spr.spr_y = 0;
    spr.dst_x = 0;
    spr.dst_y = 0;
    spr.wid = 500;
    spr.hei = 500;
    spr.scale_x = 1.0f;
    spr.scale_y = 1.0f;

    sprite_map[file] = spr;
  }

  g_camera->track_pos(&pos);

  g_input_manager->bind_mouse(&mouse_clicked, nullptr, &mouse_wheel_clicked);
}

void Game::fixed_update(double tmod) {
}

void Game::update(double dt) {
  m_cooldown->update(dt);

  invisible_ratio = grid_ratio * g_camera->get_game_scale();
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {}

void Game::draw_ent() {
  if(m_selected_asset.file_name != ""){
    auto spr = m_selected_asset.spr;
    auto tex = *g_res->get_texture(m_selected_asset.file_name);
    g_renderer->draw(tex, spr, {0, 0});
  }
}

void Game::draw_ui() {
  if (selected_asset != "") {
    auto spr = sprite_map[selected_asset];
    GPU_Image* tex = *g_res->get_texture(selected_asset);
    spr.wid = tex->w;
    spr.hei = tex->h;
    g_renderer->draw(tex, spr, {0, 100});

    auto w = tex->w*g_camera->get_game_scale();
    auto h = tex->h*g_camera->get_game_scale();

    for(int i = 0; i < w; i += invisible_ratio.x){
      for(int j = 0; j < h; j += invisible_ratio.y){
        auto mouse_hover = Mouse::is_at_area({i, 100+j, invisible_ratio.x, invisible_ratio.y}, g_engine->get_window_size()->x, g_engine->get_window_size()->y);
        Col color = {255, 255, 255, 45};
        auto fill = false;

        if(mouse_hover){
          color = {0, 255, 0, 85};
          fill = true;

          if(mouse_clicked){
            Asset asset;
            asset.file_name = selected_asset;
            Sprite spr;
            spr.dst_x = i;
            spr.dst_y = j;
            spr.wid = grid_ratio.x;
            spr.hei = grid_ratio.y;

            asset.spr = spr;
            m_assets.push_back(asset);
            mouse_clicked = false;
          }
        }


        g_renderer->draw_rect({i, 100+j, static_cast<int>(invisible_ratio.x), static_cast<int>(invisible_ratio.y)}, color, fill);
      }
    }
  }
}

void Game::imgui_assets() {}

int x = 16;
int y = 16;
void Game::imgui_map() {
  ImGui::Begin("Assets");
  for(auto& asset : m_assets){
    if (ImGui::Button(asset.file_name.c_str())) {
      m_selected_asset = asset;
    }
  }
  ImGui::End();

  ImGui::Begin("Sprites");
  ImGui::InputInt("Grid ratio x", &x);
  ImGui::InputInt("Grid ratio y", &y);
  grid_ratio = {static_cast<float>(x), static_cast<float>(y)};
  for (auto &[key, value] : sprite_map) {
    if (ImGui::Button(key.c_str())) {
      selected_asset = key;
    }
  }
  ImGui::End();

  ImGui::Begin("Mouse info");
  ImGui::Text("Mouse pos: %f %f", mouse_pos.x, mouse_pos.y);
  ImGui::Text("Mouse state : %d", mouse_clicked);
  ImGui::End();
}

void Game::draw_imgui() {
  GUI::draw([this]() { this->imgui_map(); });
}

void Game::clean() {}
