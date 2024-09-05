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

  test.dst_x = 0;
  test.dst_y = 0;
  test.wid = 500;
  test.hei = 500;
  test.scale_x = 0.1f;
  test.scale_y = 0.1f;


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
  g_renderer->draw(*g_res->get_texture("framework"), test, pos);
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


