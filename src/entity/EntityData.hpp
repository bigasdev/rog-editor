#pragma once 
#include "IData.hpp"
#include "../tools/Common.hpp"
#include <string>

class EntityData : public IData
{
public:
    EntityData();
    virtual void update();
    virtual void dispose();

    std::string name;
    std::string pallete_name;
    std::string group;
    vec2i sprite_size;
    vec2i sprite_pos;
    vec2i atlas_pos;
private:
};
