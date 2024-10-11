#pragma once 

#include <map>
#include <string>
#include <vector>
class Sprite;
class ase_t;

struct Pallete{
  Sprite *sprite;
  ase_t *ase;
};

class AssetView
{
public:
    AssetView(std::map<std::string, Sprite> sprites);

    void show();
private:
    void entities();
    void atlas();
    void pallete();

    std::vector<std::string> m_groups;
};
