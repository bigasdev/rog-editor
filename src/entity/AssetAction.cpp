#include "AssetAction.hpp"
#include "EntityData.hpp"
#include "../tools/Logger.hpp"
#include "../core/global.hpp"
#include <algorithm>
#include <string>
#include <map>
#include "../tools/boxer.h"

AssetAction::~AssetAction() {}

void AssetAction::execute() {
  m_asset_entities[m_entity.name] = m_entity;
  g_selected_entity = &m_asset_entities[m_entity.name];
}

void AssetAction::undo() {
  for(auto it = m_asset_entities.begin(); it != m_asset_entities.end(); ++it) {
    if((it->first) == m_entity.name) {
      boxer::show(("Are you sure you want to undo : " + m_entity.name).c_str(), "Undo", boxer::Style::Warning, boxer::Buttons::YesNo);
      Logger::log("Undoing asset " + m_entity.name);
      m_asset_entities.erase(it);
      break;
    }
  }
}
