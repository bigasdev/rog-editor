#include "Game.hpp"
#include "../renderer/AppGui.hpp"
#include "DataLoader.hpp"
#include "../imgui/imgui.h"
#include "../renderer/Camera.hpp"
#include "../tools/Cooldown.hpp"
#include "../core/Engine.hpp"
#include "../core/InputManager.hpp"
#include "../renderer/Renderer.hpp"
#include "../res/Res.hpp"
#include "../core/global.hpp"
#include "../core/Timer.hpp"
#include "../tools/Math.hpp"
#include "../renderer/Sprite.hpp"
#include "../tools/Logger.hpp"
#include "SDL.h"

std::string project_folder;
Sprite test;
vec2 pos = {0,0};
std::map<std::string, Sprite> sprite_map;

Game::Game() {
}

Game::~Game() {
}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  project_folder = Data_Loader::load_folder("Select project folder");
  g_res->reset_aseprites();
  g_res->load_aseprites(project_folder + "/res/");

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
    spr.scale_x = 0.65f;
    spr.scale_y = 0.65f;

    sprite_map[file] = spr;
    x += 50;
  }

  g_camera->track_pos(&pos);
}

void Game::fixed_update(double tmod) {
}

void Game::update(double dt) {
  m_cooldown->update(dt);
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
}

void Game::draw_ent(){
  for (auto& [key, value] : sprite_map) {
    g_renderer->draw(*g_res->get_texture(key), value, {0,0});
  }
}

void Game::draw_ui(){

}

void Game::draw_imgui() {
  GUI::draw([&]() {
    ImGui::Begin("Game");
    ImGui::Text(project_folder.c_str());
    ImGui::End();
  });
}

void Game::clean() {
}


