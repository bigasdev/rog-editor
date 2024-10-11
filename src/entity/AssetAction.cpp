#include "AssetAction.hpp"
#include "EntityData.hpp"
#include "../tools/Logger.hpp"
#include <algorithm>
#include <string>
#include <map>

AssetAction::~AssetAction() {}

void AssetAction::execute() {
  m_asset_entities[m_entity.name] = m_entity;
}

void AssetAction::undo() {
  for(auto it = m_asset_entities.begin(); it != m_asset_entities.end(); ++it) {
    if((it->first) == m_entity.name) {
      Logger::log("Undoing asset " + m_entity.name);
      m_asset_entities.erase(it);
      break;
    }
  }
}
