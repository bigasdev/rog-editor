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
  int x = 0;
  for (int i = 384; i < g_engine->get_window_size()->x - 32;
       i += (m_grid_size.x * m_zoom)) {
    for (int j = 48; j < g_engine->get_window_size()->y - 48;
         j += (m_grid_size.y * m_zoom)) {
      if (x == 1) {
        g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x * m_zoom),
                               static_cast<int>(m_grid_size.y * m_zoom)},
                              {82, 250, 161, 55}, true);
        x = 0;
      } else {
        g_renderer->draw_rect({i, j, static_cast<int>(m_grid_size.x * m_zoom),
                               static_cast<int>(m_grid_size.y * m_zoom)},
                              {35, 205, 113, 65}, true);
        x++;
      }
    }
  }
  if (g_selected_entity == nullptr)
    return;

  g_renderer->draw_from_sheet(
      *g_res->get_texture(g_selected_entity->pallete_name), {(static_cast<float>(734))-(g_selected_entity->sprite_size.x*m_zoom), (static_cast<float>(321))-(g_selected_entity->sprite_size.y*m_zoom)},
      {g_selected_entity->atlas_pos.x, g_selected_entity->atlas_pos.y,
       g_selected_entity->sprite_size.x, g_selected_entity->sprite_size.y},
      m_zoom);
}

void AssetScreen::ent() {}

void AssetScreen::ui() {}
