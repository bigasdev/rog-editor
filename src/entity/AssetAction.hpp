#pragma once 

#include "IAction.hpp"

#include <string>
#include <map>
#include "EntityData.hpp"

class AssetAction :  public IAction{
public:
  AssetAction(EntityData entity, std::map<std::string, EntityData>& asset_entities) : m_entity(entity), m_asset_entities(asset_entities) {}
  ~AssetAction();

  void execute() override;
  void undo() override;
private:
  EntityData m_entity;
  std::map<std::string, EntityData>& m_asset_entities;
};
