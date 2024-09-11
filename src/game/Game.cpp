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
#include "Fini.hpp"
#include "SDL.h"
#include "SDL_gpu.h"
#include "SDL_keycode.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <memory>
#include <string>

struct Animation{
  std::string name;
  int starting_x = 0;
  int frames = 0;
  float frame_speed = 0.0f;
};

struct Asset {
  char asset_name[128] = "asset";
  std::string file_name;
  Sprite spr;
  std::vector<Animation> animations;
};

std::string project_folder;
std::string asset_folder;
vec2 pos = {0, 0};
std::map<std::string, Sprite> sprite_map;
std::vector<std::unique_ptr<Asset>> m_assets;
std::unique_ptr<Asset> *m_selected_asset;
std::string selected_asset;
std::string current_asset = "asset";

vec2 mouse_pos;
bool mouse_clicked;
bool mouse_wheel_clicked;

vec2 grid_ratio = {16, 16};
vec2 invisible_ratio = {16, 16};
float sprite_zoom = 3;

bool ctrl_pressed = false;
bool load_project = false;
bool save_pressed = false;
bool load_assets = false;

SpriteAnimator *m_sprite_animator;

Fini *fini;

Game::Game() {}

Game::~Game() {}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();

  // initial settings to get last folder and asset
  fini = new Fini("res/config.ini");
  fini->initialize_value("last", "folder", "");
  fini->initialize_value("last", "asset", "");

  project_folder = fini->get_value<std::string>("last", "folder");

  // this will crash if the saved folder or assets are not found anymore
  // FIX: 
  if (project_folder != "") {
    g_res->reset_aseprites();
    g_res->load_aseprites(project_folder + "/res/");
  }

  asset_folder = fini->get_value<std::string>("last", "asset");
  if (asset_folder != "") {
    load(asset_folder);
  }

  g_cooldown = m_cooldown;
  g_camera = m_camera;

  auto files = g_res->get_aseprite_names();
  for (auto file : files) {
    Logger::log(file);

    auto spr = Sprite();

    sprite_map[file] = spr;
  }

  // basic camera tracking to make everything in the middle of the screen
  g_camera->track_pos(&pos);

  g_input_manager->bind_mouse(&mouse_clicked, nullptr, &mouse_wheel_clicked);
  g_input_manager->bind_keyboard(SDLK_s, &save_pressed);
  g_input_manager->bind_keyboard(SDLK_p, &load_assets);
  g_input_manager->bind_keyboard(SDLK_e, &load_project);
  g_input_manager->bind_keyboard(SDLK_LCTRL, &ctrl_pressed);

  //test stuff 
  m_sprite_animator = new SpriteAnimator(Sprite());
  SpriteFrame frame;
  frame.max_frames = 3;
  frame.frame_timer = 0.16f;
  frame.loop = true;
  SpriteFrame frame2;
  frame2.max_frames = 6;
  frame2.frame_timer = 0.16f;
  frame2.loop = true;

  m_sprite_animator->register_anim(frame, &mouse_clicked);
  m_sprite_animator->register_anim(frame2, &mouse_wheel_clicked);
}

void Game::fixed_update(double tmod) {}

void Game::update(double dt) {
  m_cooldown->update(dt);
  m_sprite_animator->update(dt);

  invisible_ratio = grid_ratio * g_camera->get_game_scale();

  // LOAD PROJECT FOLDER
  if (ctrl_pressed and load_project) {
    project_folder = Data_Loader::load_folder("Select project folder");
    if (project_folder == "")
      return;
    fini->set_value("last", "folder", project_folder);
    g_res->reset_aseprites();
    g_res->load_aseprites(project_folder + "/res/");
    sprite_map.clear();

    auto files = g_res->get_aseprite_names();
    for (auto file : files) {
      Logger::log(file);

      auto spr = Sprite();

      sprite_map[file] = spr;
    }

    load_project = false;
    ctrl_pressed = false;
  }

  // SAVE ASSETS
  if (ctrl_pressed and save_pressed) {
    save();
    save_pressed = false;
    ctrl_pressed = false;
  }

  // LOAD ASSETS
  if (ctrl_pressed and load_assets) {
    auto file_path = Data_Loader::load_file("*.json");
    if (file_path == "")
      return;
    load(file_path);
    fini->set_value("last", "asset", file_path);
    load_assets = false;
    ctrl_pressed = false;
  }
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
  // draws the background of the sprite showing the current grid ratio respecting the zoom
  g_renderer->draw_rect(
      {0, 0, static_cast<int>(grid_ratio.x * sprite_zoom), static_cast<int>(grid_ratio.y * sprite_zoom)},
      {255, 255, 255, 55}, true);
  g_renderer->draw_line({0, static_cast<int>((grid_ratio.y * sprite_zoom)) + 8,
                         static_cast<int>(grid_ratio.x * sprite_zoom),
                         static_cast<int>((grid_ratio.y * sprite_zoom) + 8)},
                        {255, 255, 255, 255});
  g_renderer->draw_text(
      {0, (grid_ratio.y * sprite_zoom) + 10},
      (std::to_string((grid_ratio.x * sprite_zoom)) + " px").c_str(),
      g_res->get_font("arial"), {255, 255, 255, 255});
  g_renderer->draw_line({static_cast<int>((grid_ratio.x * sprite_zoom) + 8), 0,
                         static_cast<int>((grid_ratio.x * sprite_zoom) + 8),
                         static_cast<int>(grid_ratio.y * sprite_zoom)},
                        {255, 255, 255, 255});
  g_renderer->draw_text(
      {(grid_ratio.x * sprite_zoom) + 10, 0},
      (std::to_string((grid_ratio.y * sprite_zoom)) + " px").c_str(),
      g_res->get_font("arial"), {255, 255, 255, 255});
}

void Game::draw_ent() {
  // draws the selected sprite in the screen respecting the sprite zoom
  if (m_selected_asset != nullptr) {
    auto spr = m_selected_asset->get()->spr;
    auto tex = *g_res->get_texture(m_selected_asset->get()->file_name);
    spr.scale_x = sprite_zoom;
    spr.scale_y = sprite_zoom;
    g_renderer->draw(tex, spr, {0, 0});
  }
}

void Game::draw_ui() {}

void Game::imgui_assets() {}

int x = 16;
int y = 16;
void Game::imgui_map() {
  //menu that contains all the assets found in the project .json
  ImGui::Begin("Assets");
  for (auto &asset : m_assets) {
    if (ImGui::Button(asset.get()->asset_name)) {
      m_selected_asset = &asset;
    }
  }
  ImGui::End();

  //atlas menu, this is where the selected atlas will be shown
  ImGui::SetNextWindowBgAlpha(0.15f);
  ImGui::Begin("Atlas");
  if (selected_asset != "") {
    auto spr = sprite_map[selected_asset];
    GPU_Image *tex = *g_res->get_texture(selected_asset);
    spr.wid = tex->w;
    spr.hei = tex->h;
    g_renderer->draw(tex, spr,
                     {ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 100});

    auto w = tex->w * g_camera->get_game_scale();
    auto h = tex->h * g_camera->get_game_scale();

    //loop used to create the grids around the texture respecting the zoom
    for (int i = 0; i < w; i += invisible_ratio.x) {
      for (int j = 0; j < h; j += invisible_ratio.y) {
        auto mouse_hover = Mouse::is_at_area(
            {static_cast<int>(ImGui::GetWindowPos().x + i), static_cast<int>(j + ImGui::GetWindowPos().y + 100),
             static_cast<int>(invisible_ratio.x), static_cast<int>(invisible_ratio.y)},
            g_engine->get_window_size()->x, g_engine->get_window_size()->y);
        Col color = {255, 255, 255, 45};
        auto fill = false;

        if (mouse_hover) {
          color = {0, 255, 0, 85};
          fill = true;

          if (mouse_clicked) {
            if (m_selected_asset != nullptr)
              m_selected_asset = nullptr;

            Asset asset;
            asset.file_name = selected_asset;
            Sprite spr;
            spr.dst_x = i / invisible_ratio.x;
            spr.dst_y = j / invisible_ratio.y;
            spr.wid = grid_ratio.x;
            spr.hei = grid_ratio.y;

            asset.spr = spr;
            m_assets.push_back(std::make_unique<Asset>(asset));
            mouse_clicked = false;
          }
        }

        g_renderer->draw_rect({static_cast<int>(ImGui::GetWindowPos().x + i),
                               static_cast<int>(j + ImGui::GetWindowPos().y + 100),
                               static_cast<int>(invisible_ratio.x),
                               static_cast<int>(invisible_ratio.y)},
                              color, fill);
      }
    }
  }
  ImGui::End();

  //asset menu that is used to add animations and control the size and position of the asset
  if (m_selected_asset != nullptr) {
    ImGui::Begin("Selected asset");
    ImGui::InputText("asset_name", m_selected_asset->get()->asset_name,
                     IM_ARRAYSIZE(m_selected_asset->get()->asset_name));
    ImGui::DragFloat("dst_x", &m_selected_asset->get()->spr.dst_x, 0.1f);
    ImGui::DragFloat("dst_y", &m_selected_asset->get()->spr.dst_y, 0.1f);
    ImGui::DragFloat("wid", &m_selected_asset->get()->spr.wid, 0.1f);
    ImGui::DragFloat("hei", &m_selected_asset->get()->spr.hei, 0.1f);
    if (ImGui::Button("Done")) {
      m_selected_asset = nullptr;
    }
    ImGui::End();
  }

  //the sprites menu, this will have all the avaliable atlases to select 
  //it contains all the control variables as well
  ImGui::Begin("Sprites");
  ImGui::InputInt("Grid ratio x", &x);
  ImGui::InputInt("Grid ratio y", &y);
  ImGui::InputFloat("Sprite zoom", &sprite_zoom);
  grid_ratio = {static_cast<float>(x), static_cast<float>(y)};
  for (auto &[key, value] : sprite_map) {
    if (ImGui::Button(key.c_str())) {
      selected_asset = key;
      m_selected_asset = nullptr;
    }
  }
  ImGui::End();
}

void Game::draw_imgui() {
  GUI::draw([this]() { this->imgui_map(); });
}

void Game::save() {
  nlohmann::json j;
  for (auto &asset : m_assets) {
    nlohmann::json asset_j;
    asset_j["asset_name"] = asset->asset_name;
    asset_j["file_name"] = asset->file_name;
    asset_j["dst_x"] = asset->spr.dst_x;
    asset_j["dst_y"] = asset->spr.dst_y;
    asset_j["wid"] = asset->spr.wid;
    asset_j["hei"] = asset->spr.hei;

    j.push_back(asset_j);
  }
  std::ofstream o;
  if (asset_folder == "") {
    std::ofstream o("res/map.json");
    o << std::setw(4) << j << std::endl;
    o.close();
  } else {
    std::ofstream o(asset_folder);
    o << std::setw(4) << j << std::endl;
    o.close();
  }
}

void Game::load(std::string file_path) {
  std::ifstream i(file_path);
  nlohmann::json j;
  i >> j;
  m_selected_asset = nullptr;
  m_assets.clear();

  for (auto &asset_j : j) {
    Asset asset;
    std::strcpy(asset.asset_name,
                asset_j["asset_name"].get<std::string>().c_str());
    asset.file_name = asset_j["file_name"];
    asset.spr.dst_x = asset_j["dst_x"];
    asset.spr.dst_y = asset_j["dst_y"];
    asset.spr.wid = asset_j["wid"];
    asset.spr.hei = asset_j["hei"];

    m_assets.push_back(std::make_unique<Asset>(asset));
  }
}

void Game::clean() { delete fini; }
