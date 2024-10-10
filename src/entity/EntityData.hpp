#pragma once 
#include "IData.hpp"
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
private:
};
