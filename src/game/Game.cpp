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

//Systems
#include "../core/UndoManager.hpp"

//Components
#include "../entity/SideMenu.hpp"
#include "../entity/MainMenu.hpp"
#include "../entity/AssetView.hpp"
#include "../entity/AssetScreen.hpp"

struct Animation {
  std::string name;
  int starting_x = 0;
  int starting_y = 0;
  int frames = 0;
  float frame_speed = 0.16f;
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
bool mouse_not_clicked;

bool ctrl_pressed = false;
bool load_project = false;
bool save_pressed = false;
bool z_pressed = false;
bool load_assets = false;

//Systems
SpriteAnimator *m_sprite_animator;
Fini *fini;
UndoManager *m_undo_manager;

//Components
std::unique_ptr<SideMenu> side_menu;
std::unique_ptr<MainMenu> main_menu;
std::unique_ptr<AssetView> asset_view;
std::unique_ptr<AssetScreen> asset_screen;

Game::Game() {}

Game::~Game() {}

void Game::init() {
  m_camera = new Camera(g_engine->get_window_size());
  m_cooldown = new Cooldown();
  m_undo_manager = new UndoManager();
  g_undo_manager = m_undo_manager;

  //

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
  g_input_manager->bind_keyboard(SDLK_z, &z_pressed);
  g_input_manager->bind_keyboard(SDLK_LCTRL, &ctrl_pressed);

  // test stuff
  // TODO: create a note or a doc about how the animation works before cleaning
  // this up..
  m_sprite_animator = new SpriteAnimator(Sprite());
  SpriteFrame frame;
  frame.name = "movement";
  frame.max_frames = 3;
  frame.frame_timer = 0.16f;
  frame.loop = true;
  frame.state = &mouse_clicked;
  SpriteFrame frame2;
  frame2.name = "click";
  frame2.max_frames = 6;
  frame2.frame_timer = 0.46f;
  frame2.loop = false;
  frame2.block_transition = true;
  frame2.state = &mouse_wheel_clicked;
  SpriteFrame frame3;
  frame3.name = "idle";
  frame3.max_frames = 5;
  frame3.frame_timer = 0.16f;
  frame3.loop = true;
  frame3.state = &mouse_not_clicked;

  //m_sprite_animator->register_anim(frame);
  //m_sprite_animator->register_anim(frame3);
  //m_sprite_animator->register_anim(frame2);


  side_menu = std::make_unique<SideMenu>(); 
  main_menu = std::make_unique<MainMenu>();
  asset_view = std::make_unique<AssetView>(sprite_map);
  asset_screen = std::make_unique<AssetScreen>();
}

void Game::fixed_update(double tmod) {}

void Game::update(double dt) {
  m_cooldown->update(dt);
  m_sprite_animator->update(dt);

  mouse_not_clicked = !mouse_clicked;

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

  // UNDO
  if (ctrl_pressed and z_pressed) {
    m_undo_manager->undo();
    z_pressed = false;
    ctrl_pressed = false;
  }

  // components updates 
  asset_screen->update();
}

void Game::post_update(double dt) {
  m_camera->move();
  m_camera->update();
}

void Game::draw_root() {
  asset_screen->root();
}

void Game::draw_ent() {
  asset_screen->ent();
}

void Game::draw_ui() {
  asset_screen->ui();
}

void Game::imgui_assets() {}

int x = 16;
int y = 16;
void Game::imgui_map() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 0.0));
  ImGui::SetNextWindowSize(ImVec2(g_engine->get_window_size()->x, g_engine->get_window_size()->y));
  ImGui::Begin("Workspace", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoScrollbar);

  side_menu->show();
  //
  main_menu->show();

  if(side_menu->get_state() == State::ASSET){
    asset_view->show();
  }

  if (side_menu->get_state() == State::NONE) {
  }
  ImGui::PopStyleColor();
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
