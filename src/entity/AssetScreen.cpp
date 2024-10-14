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
    for (int i = 380; i < g_engine->get_window_size()->x - 40;
         i += (16 * m_zoom)) {
      for (int j = 50; j < g_engine->get_window_size()->y - 40;
           j += (16 * m_zoom)) {
        if (g_selected_entity->name == "asset") {
          if (i % 2 == 0 || j % 2 == 0) {
            g_renderer->draw_rect({i, j, 16, 16}, {82, 250, 161, 155}, true);
          } else {
            g_renderer->draw_rect({i, j, 16, 16}, {35, 205, 113, 165}, true);
          }
        }
      }
    }
  }
  for(int i = 320; i < g_engine->get_window_size()->x -40; i+= (16*m_zoom)){
    g_renderer->draw_rect({i, 50, 16, 16}, {255, 0, 0, 255}, true);
  }
  g_renderer->draw_text({20, 20}, "Test", g_res->get_font("arial"), {255, 255, 255, 255}, 1, 200);
}

void AssetScreen::ent() {
}

void AssetScreen::ui() {
}
