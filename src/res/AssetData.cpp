#include "AssetData.hpp"

AssetData::AssetData(std::string name, std::string folder, GPU_Image *texture)
{
  m_name = name;
  m_folder = folder;
  m_texture = texture;
}

AssetData::~AssetData()
{
}

std::string AssetData::GetName()
{
  return m_name;
}

void AssetData::set_size(vec2 size)
{
  m_size = size;
}

std::string AssetData::GetFolder()
{
  return m_folder;
}

GPU_Image *AssetData::GetTexture()
{
  return m_texture;
}

vec2 AssetData::get_size()
{
  return m_size;
}
