#include "Game.hpp"
#include "../renderer/AppGui.hpp"
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


Game::Game() {
}

Game::~Game() {
}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  g_cooldown = m_cooldown;
  g_camera = m_camera;
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
}

void Game::draw_ui(){

}

void Game::draw_imgui() {
  GUI::draw([&]() {
    ImGui::Begin("Game");
    ImGui::Text("Hello, world!");
    ImGui::End();
  });
}

void Game::clean() {
}


