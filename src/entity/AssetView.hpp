#pragma once 

#include <map>
#include <string>
#include <vector>
class Sprite;

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
