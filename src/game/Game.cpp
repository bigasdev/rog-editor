#include "Game.hpp"
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
#include "SDL.h"

bool moving_right = false;
bool moving_left = false;
bool slow_mo = false;

int hero_x = 2;
float timer = 0;

float dx, dy, dwood;
vec2 hero_pos;
vec2 wood_pos = {20, 40};

Sprite hero;

Game::Game() {
}

Game::~Game() {
}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  g_input_manager->bind_keyboard(SDLK_e, &moving_right);
  g_input_manager->bind_keyboard(SDLK_q, &moving_left);
  g_input_manager->bind_keyboard(SDLK_SPACE, &slow_mo);

  g_camera->track_pos(&hero_pos);

  hero.dst_x = 0;
  hero.dst_y = 1;
  hero.wid = 7;
  hero.hei = 8;
}

void Game::fixed_update(double tmod) {
  dx += (g_input_manager->get_raw_axis().x * 17.5) * tmod;
  dy += (g_input_manager->get_raw_axis().y * 17.5) * tmod;

  dx*=Math::pow(.82f, tmod);
  dy*=Math::pow(.82f, tmod);
}

void Game::update(double dt) {
  m_cooldown->update(dt);

  if (moving_left)
    m_camera->track_pos(&wood_pos);
  if(moving_right)
    m_camera->track_pos(&hero_pos);

  if(dx > 0){
    hero.dir = 1;
  }else if(dx < 0){
    hero.dir = -1;
  }

  timer += 1*dt;
  if(timer >= .1f){
    hero_x++;
    if(hero_x >= 6){
      hero_x = 2;
    }
    timer = 0;
  }

  if(slow_mo){
    Timer::apply_slow_mo(.1f * dt);
  }
  
  hero_pos += {dx,dy};
  wood_pos += {0, dwood};
  if(wood_pos.y > 70){
    wood_pos.y = 0;
  }
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
  for (int i = -500; i < 500; i += 8*g_camera->get_game_scale()) {
    for (int j = -500; j < 500; j += 8*g_camera->get_game_scale()) {
      g_renderer->draw_from_sheet(*g_res->get_texture("concept"), {i, j},
                                  {2, 0, 8, 8}, false);
    }
  }
}

void Game::draw_ent(){
  g_renderer->draw(*g_res->get_texture("concept"), hero, hero_pos);
  g_renderer->draw_from_sheet(*g_res->get_texture("concept"),wood_pos,
                              {1, 1, 31, 16}, true);
  g_renderer->draw_from_sheet(*g_res->get_texture("concept"),{-15,30},
                              {0, 3, 8, 8}, true);
}
void Game::draw_ui(){

}

void Game::clean() {
}


