#pragma once 

#include <map>
#include <string>
class Sprite;

class AssetView
{
public:
    AssetView(std::map<std::string, Sprite> sprites);

    void show();
private:
    void pallete();
};
