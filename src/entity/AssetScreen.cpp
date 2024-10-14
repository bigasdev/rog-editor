#include "AssetScreen.hpp"
#include "../core/Engine.hpp"
#include "../core/global.hpp"
#include "../renderer/Renderer.hpp"
#include "../res/Res.hpp"
#include "../tools/Logger.hpp"
#include "EntityData.hpp"

AssetScreen::AssetScreen() {}

AssetScreen::~AssetScreen() {}

void AssetScreen::update() {
  if (g_selected_entity != nullptr) {
  }
}

void AssetScreen::root() {
  if (g_selected_entity == nullptr)
    return;
  int x = 0;
  for (int i = 380; i < g_engine->get_window_size()->x - 40;
       i += (m_grid_size.x * m_zoom)) {
    for (int j = 50; j < g_engine->get_window_size()->y - 40;
         j += (m_grid_size.y * m_zoom)) {
      if (x == 1) {
        g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x*m_zoom),
                               static_cast<int>(m_grid_size.y*m_zoom)},
                              {82, 250, 161, 55}, true);
        x = 0;
      } else {
        g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x*m_zoom),
                               static_cast<int>(m_grid_size.y*m_zoom)},
                              {35, 205, 113, 65}, true);
        x++;
      }
    }
  }
  g_renderer->draw_from_sheet(
      *g_res->get_texture(g_selected_entity->pallete_name), {0, 0},
      {g_selected_entity->sprite_pos.x/(g_selected_entity->sprite_size.x*m_zoom), g_selected_entity->sprite_pos.y/(g_selected_entity->sprite_size.y*m_zoom),
       g_selected_entity->sprite_size.x*m_zoom, g_selected_entity->sprite_size.y*m_zoom});
}

void AssetScreen::ent() {}

void AssetScreen::ui() {}
