#include "AssetScreen.hpp"
#include "../core/Engine.hpp"
#include "../core/global.hpp"
#include "../renderer/Renderer.hpp"
#include "../res/Res.hpp"
#include "EntityData.hpp"

AssetScreen::AssetScreen() {}

AssetScreen::~AssetScreen() {}

void AssetScreen::update() {
  if (g_selected_entity != nullptr) {
  }
}

void AssetScreen::root() {
  if (g_selected_entity != nullptr) {
    int x = 0;
    for (int i = 380; i < g_engine->get_window_size()->x - 40;
         i += (16 * m_zoom)) {
      for (int j = 50; j < g_engine->get_window_size()->y - 40;
           j += (16 * m_zoom)) {
        if (x == 1) {
          g_renderer->draw_rect({i, j, 16, 16}, {82, 250, 161, 55}, true);
          x = 0;
        } else {
          g_renderer->draw_rect({i, j, 16, 16}, {35, 205, 113, 65}, true);
          x++;
        }
      }
    }
  }
}

void AssetScreen::ent() {}

void AssetScreen::ui() {}
